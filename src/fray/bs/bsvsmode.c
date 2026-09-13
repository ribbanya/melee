#include "bsvsmode.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/lb/forward.h>
#include <melee/mn/forward.h>
#include <melee/pl/forward.h>

#include <string.h>

#include "bisimulation.h"
#include "bsdisplay.h"
#include "fray/bs/bsarchive.h"
#include "fray/bs/bshash.h"
#include "fray/bs/bsio.h"
#include "melee/gm/gmvs.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/gm/gm_1601.h>
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

static void setupSnapshot(void)
{
    memset(&snapshot, 0, sizeof(snapshot));
    memset(&save_data, 0, sizeof(save_data));
    bsIO_Init(&snapshot_cursor, (u8*) &curr_snapshot, sizeof(curr_snapshot));
}

static void updateSnapshot(void)
{
    u32 f = gm_GetFrameCount();
    u32 h;

    if (curr_frame == f) {
        return;
    }

    curr_frame = f;

    Bisim_CaptureGlobal(&snapshot);

    bsIO_Reset(&snapshot_cursor);
    Bisim_WriteGlobal(&snapshot_cursor, &snapshot);
    FRAY_ASSERT(snapshot_cursor.err == BsIO_Ok);

    h = bsHash_Cursor(bsHash_Init(), &snapshot_cursor);
    save_data.seeded_hashes.hashes[curr_frame] = h;
    BsDisplay_Draw(&snapshot, h);

    if (curr_frame == 0) {
        memcpy(save_data.start_snapshot, &snapshot,
               sizeof(save_data.start_snapshot));
    } else if (curr_frame == end_frame) {
        /// @todo on match end
        memcpy(save_data.end_snapshot, &snapshot,
               sizeof(save_data.end_snapshot));
    }
    FRAY_ASSERT(curr_frame <= end_frame);
}

static void onMatchStartRecordVs(void)
{
    setupSnapshot();
    BsDisplay_Init();
    BsDisplay_Show();
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

void Replay_Mode_OnLoad(void) {}

void Replay_Mode_OnUnload(void) {}

static void onRecordVsStartMelee(StartMeleeData* start,
                                 UNUSED StartMeleeData* vs)
{
    start->rules.match_kind = MatchKind_Time;
    start->rules.time_limit = BISIM_MAX_SECONDS;
    curr_frame = U32_MAX;
    save_data.seeded_hashes.seed = *HSD_RandSeedPtr;
    end_frame = BISIM_MAX_SECONDS * GM_FPS;

    REPORT_HEX(save_data.seeded_hashes.seed);
    REPORT_UINT(end_frame);
}

void onEnterRecordVs(GameModeState* state)
{
    gmVsMelee_EnterVs(state, &vs_mode_data, onRecordVsStartMelee, NULL);
}

void onExitRecordVs(GameModeState* state)
{
    gmVsMelee_ExitVs(state, state_record_over, state_record_over);
}

void onEnterRecordOver(UNUSED GameModeState* state)
{
    // BsDisplay_Init();
    // BsDisplay_Show();
    // BsDisplay_Draw(&archive);
}

void onExitRecordOver(UNUSED GameModeState* state) {}
