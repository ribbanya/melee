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
#include "melee/gm/gmmain_lib.h"
#include "melee/lb/lbcardgame.h"
#include "melee/lb/lbcardnew.h"
#include "sysdolphin/baselib/debug.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gmvs.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <sysdolphin/baselib/random.h>

#define GAME_SPEED 1.0f

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);

static void onEnterRecordOver(GameModeState* state);
static void onExitRecordOver(GameModeState* state);
static VsModeData vs_mode_data;

static Bisim_GlobalSnapshot snapshot;

static u32 curr_frame;
static u32 end_frame;

static Bisim_GlobalBuf curr_snapshot;
static BsIO_Cursor snapshot_cursor;
static Bisim_SaveData save_data;

enum {
    state_record_vs,
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
    { state_record_over,
      lbDvdPreload_2,
      0,
      onEnterRecordOver,
      onExitRecordOver,
      {
          GS_COMING_SOON,
          NULL,
          NULL,
      }

    },
    { GM_GAMEMODESTATE_TERMINATE },
};

Bisim_SaveData* bsVsMode_GetSaveData(void)
{
    return &save_data;
}

static void setupSnapshot(void)
{
    memset(&snapshot, 0, sizeof(snapshot));
    memset(&save_data, 0, sizeof(save_data));
    bsIO_Init(&snapshot_cursor, (u8*) &curr_snapshot, sizeof(curr_snapshot));
}

static void writeSnapshot(Bisim_ArchiveHeader* ah, Bisim_GlobalBuf* dst)
{
    bsIO_Init(&snapshot_cursor, (u8*) dst, sizeof(*dst));
    Bisim_WriteGlobal(&snapshot_cursor, &snapshot);
    bsArchive_SetHeader(ah, &snapshot_cursor, BisimBlob_GlobalSnapshot, 0);
    FRAY_ASSERT(snapshot_cursor.err == BsIO_Ok);
}

static void writeRecording(Bisim_ArchiveHeader* ah, Bisim_SeededHashes* dst)
{
    bsIO_Init(&snapshot_cursor, (u8*) dst, sizeof(*dst));
    snapshot_cursor.pos += sizeof(*dst);
    bsArchive_SetHeader(ah, &snapshot_cursor, BisimBlob_SeededHashes, 0);
    FRAY_ASSERT(snapshot_cursor.err == BsIO_Ok);
}

static void updateSnapshot(void)
{
    {
        u32 f = gm_GetFrameCount();
        if (curr_frame == f) {
            return;
        }
        curr_frame = f;
    }

    Bisim_CaptureGlobal(&snapshot);

    {
        u32 h = bsHash_Cursor(bsHash_Init(), &snapshot_cursor);
        // save_data.history.hashes[curr_frame] = h;
        save_data.history.hashes[curr_frame] = curr_frame;
        BsDisplay_Draw(&snapshot, h);
    }

    if (curr_frame == 0) {
        writeSnapshot(&save_data.start_header, &save_data.start);
    } else if (curr_frame == end_frame) {
        writeSnapshot(&save_data.end_header, &save_data.end);
        writeRecording(&save_data.history_header, &save_data.history);
    }
    FRAY_ASSERT(curr_frame <= end_frame);
}

static void onMatchStartRecordVs(void)
{
    Bisim_ArchiveHeader* ah = &save_data.history_header;
    setupSnapshot();
    BsDisplay_Init();
    BsDisplay_Show();

    OSReport("Loading: %.4s, 0x%X, %d, %d, 0x%08X, %u, 0x%08X\n", &ah->magic,
             ah->header_size, ah->version, ah->type, ah->hash, ah->size,
             ah->flags);
}

static void onFrameEndRecordVs(void)
{
    updateSnapshot();
}

/// @todo Load from memcard
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
    rules->on_frame_end = onFrameEndRecordVs;

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
    // Qol_LogInit();
    Qol_UnlockAll();
    Qol_SetCompetitivePrefs();
    gm_InitVsMode(&vs_mode_data);
    initTestMatch(&vs_mode_data.start);
}

void Replay_Mode_OnLoad(void)
{
    // lbCardNew_AllocWorkArea();
    // lbCardGame_LoadArchive(0);
}

void Replay_Mode_OnUnload(void) {}

static void onRecordVsStartMelee(StartMeleeData* start,
                                 UNUSED StartMeleeData* vs)
{
    start->rules.match_kind = MatchKind_Time;
    start->rules.time_limit = 5; // BISIM_MAX_SECONDS;
    curr_frame = U32_MAX;
    save_data.history.seed = *HSD_RandSeedPtr;
    end_frame = start->rules.time_limit * GM_FPS;

    REPORT_HEX(save_data.history.seed);
    REPORT_UINT(end_frame);
    // lbCardGame_LoadArchive(0);
}

void onEnterRecordVs(GameModeState* state)
{
    lbCardNew_AllocWorkArea();
    lbCardGame_LoadArchive(0);
    gmVsMelee_EnterVs(state, &vs_mode_data, onRecordVsStartMelee, NULL);
}

void onExitRecordVs(UNUSED GameModeState* state)
{
    lbCardGame_SaveChanges();
}

void onEnterRecordOver(UNUSED GameModeState* state) {}

void onExitRecordOver(UNUSED GameModeState* state) {}
