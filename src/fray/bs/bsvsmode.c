#include "bsvsmode.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/lb/forward.h>
#include <melee/mn/forward.h>
#include <melee/pl/forward.h>

#include <string.h>

#include "bisimulation.h"
#include "bsarchive.h"
#include "bsdisplay.h"
#include "bshash.h"
#include "bsio.h"
#include "melee/gm/gm_1A3F.h"
#include "melee/gm/gmscene.h"
#include "melee/lb/lbcardgame.h"
#include "melee/lb/lbcardnew.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gmvs.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <sysdolphin/baselib/random.h>

#define GAME_SPEED 0.5
#define FIXED_SEED 0xDEADBEEF

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);

static void onEnterValidateVs(GameModeState* state);
static void onExitValidateVs(GameModeState* state);

static void onEnterRecordOver(GameModeState* state);
static void onExitRecordOver(GameModeState* state);

static VsModeData vs_mode_data;
static Bisim_GlobalSnapshot snapshot;

static u32 curr_frame;
static u32 end_frame;

static BsIO_Cursor cursor;
static struct {
    Bisim_ArchiveHeader header;
    Bisim_GlobalBuf snapshot;
} curr;
static Bisim_SaveData save_data;

enum {
    state_record_vs,
    state_validate_vs,
    state_record_over,
};

GameModeState Replay_RecordStates[] = {
    {
        state_record_vs,
        lbDvdPreload_2,
        0,
        onEnterRecordVs,
        onExitRecordVs,
        {
            GS_VS,
            &gmVsMelee_StartData,
            &gmVsMelee_VsExitInfo,
        },
    },
    {
        state_validate_vs,
        lbDvdPreload_2,
        0,
        onEnterValidateVs,
        onExitValidateVs,
        {
            GS_VS,
            &gmVsMelee_StartData,
            &gmVsMelee_VsExitInfo,
        },
    },
    {
        state_record_over,
        lbDvdPreload_2,
        0,
        onEnterRecordOver,
        onExitRecordOver,
        {
            GS_COMING_SOON,
            NULL,
            NULL,
        },
    },
    { GM_GAMEMODESTATE_TERMINATE },
};

Bisim_SaveData* bsVsMode_GetSaveData(void)
{
    return &save_data;
}

static void processHash(u32 h)
{
    ssize_t idx = gm_GetCurrentSceneIndex();
    switch (idx) {
    case state_record_vs:
        save_data.history.hashes[curr_frame] = h;
        if (curr_frame == 0) {
            bsIO_Init(&cursor, &save_data.start, sizeof(save_data.start));
            Bisim_WriteGlobal(&cursor, &snapshot);
        } else if (curr_frame == end_frame) {
            bsIO_Init(&cursor, &save_data.history, sizeof(save_data.history));
            cursor.pos += sizeof(save_data.history);
            bsArchive_SetHeader(&save_data.history_header, &cursor,
                                BisimBlob_SeededHashes, 0);
        }
        FRAY_ASSERT(cursor.err == BsIO_Ok);
        break;
    case state_validate_vs: {
        u32 expected = save_data.history.hashes[curr_frame];
        if (h != expected) {
            OSReport("%*u: %08X != %08X", 5, curr_frame, h, expected);
        }
    } break;
    default:
        FRAY_PANIC("unexpected mode state %d", idx);
    }
}

static void onFrameEndVs(void)
{
    {
        u32 f = gm_GetFrameCount();
        if (curr_frame == f) {
            return;
        }
        curr_frame = f;
    }

    Bisim_CaptureGlobal(&snapshot);
    bsIO_Reset(&cursor);
    Bisim_WriteGlobal(&cursor, &snapshot);
    bsArchive_SetHeader(&curr.header, &cursor, BisimBlob_GlobalSnapshot, 0);
    FRAY_ASSERT(cursor.err == BsIO_Ok);

    {
        u32 h = bsHash_Cursor(bsHash_Init(), &cursor);
        BsDisplay_Draw(&snapshot, h);
        processHash(h);
    }

    FRAY_ASSERT(curr_frame <= end_frame);
}

static void onMatchStartRecordVs(void)
{
    BsDisplay_Init();
    BsDisplay_Show();
    bsIO_Init(&cursor, &curr.snapshot, sizeof(curr.snapshot));
}

static void initTestMatch(StartMeleeData* start)
{
    StartMeleeRules* rules = &start->rules;
    PlayerInitData* players = &start->players[0];
    size_t const max_players = 2;
    size_t const max_tier = 1;
    size_t const max_stage = 1;
    size_t i;

    rules->stkind = Qol_PickRandomLegalStage(max_stage);
    rules->game_speed = GAME_SPEED;

    rules->on_unpause_override = gm_80165290;
    rules->on_match_start = onMatchStartRecordVs;
    rules->on_frame_end = onFrameEndVs;

    for (i = 0; i < max_players; i++) {
        u8 ckind = Qol_PickRandomTopTier(max_tier);
        players[i].slot_type = Gm_PKind_Cpu;
        players[i].ckind = ckind;
        players[i].color = HSD_Randi(gm_GetNumCostumesForCKind(ckind));
        players[i].cpu_level = 9;
        players[i].damage = 100;
    }
}

void Replay_Mode_OnInit(void)
{
    Qol_UnlockAll();
    Qol_SetCompetitivePrefs();
    gm_InitVsMode(&vs_mode_data);
    initTestMatch(&vs_mode_data.start);
}

void bsVsMode_OnLoad(void) {}
void bsVsMode_OnUnload(void) {}

static void setupStartMelee(StartMeleeData* start)
{
    start->rules.match_kind = MatchKind_Time;
    start->rules.time_limit = BISIM_MAX_SECONDS;
    curr_frame = U32_MAX;
    end_frame = BISIM_MAX_SECONDS * GM_FPS;
}

static void onRecordVsStartMelee(StartMeleeData* start,
                                 UNUSED StartMeleeData* vs)
{
    setupStartMelee(start);
}

static void initCard(void)
{
    lbCardNew_AllocWorkArea();
    lbCardGame_LoadArchive(0);
}

void onEnterRecordVs(GameModeState* state)
{
    initCard();
    memset(&snapshot, 0, sizeof(snapshot));
    memset(&save_data, 0, sizeof(save_data));
    *HSD_RandSeedPtr = save_data.history.seed = FIXED_SEED;
    gmVsMelee_EnterVs(state, &vs_mode_data, onRecordVsStartMelee, NULL);
}

void onExitRecordVs(UNUSED GameModeState* state)
{
    lbCardGame_SaveChanges();
}

static void onValidateVsStartMelee(StartMeleeData* start,
                                   UNUSED StartMeleeData* vs)
{
    setupStartMelee(start);
}

void onEnterValidateVs(GameModeState* state)
{
    initCard();
    *HSD_RandSeedPtr = save_data.history.seed;
    gmVsMelee_EnterVs(state, &vs_mode_data, onValidateVsStartMelee, NULL);
}
void onExitValidateVs(UNUSED GameModeState* state) {}

void onEnterRecordOver(UNUSED GameModeState* state) {}
void onExitRecordOver(UNUSED GameModeState* state) {}
