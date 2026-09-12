#include "bsvsmode.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/lb/forward.h>
#include <melee/mn/forward.h>
#include <melee/pl/forward.h>

#include "bisimulation.h"
#include "bsdisplay.h"
#include "fray/bs/bsio.h"
#include "fray/lb/lbosd.h"
#include "sysdolphin/baselib/gobjproc.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <sysdolphin/baselib/random.h>

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);
static void onEnterPlaybackVs(GameModeState* state);
static void onExitPlaybackVs(GameModeState* state);

static void onExitRecordOver(GameModeState* state);
static ReplaySetupData setup_data;
static VsModeData vs_mode_data;
static HSD_GObj* replay_gobj;
static Bisim_GlobalSnapshot snapshot;
static char sprint_buf[0x400];

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
        NULL,
        {
            GS_VS,
            &gmVsMelee_StartData,
            &gmVsMelee_VsExitInfo,
        },
    },
    { state_record_over,
      lbDvdPreload_2,
      0,
      NULL,
      onExitRecordOver,
      {
          GS_COMING_SOON,
          NULL,
          NULL,
      }

    },
    { GM_GAMEMODESTATE_TERMINATE },
};

static void onMatchStartRecordVs(void)
{
    BsDisplay_Init();
    Osd_SetAlpha(0x80);
}

static void onFrameEndRecordVs(void)
{
    Bisim_CaptureGlobal(&snapshot);
    bsIO_MemZero(&sprint_buf, sizeof(sprint_buf));
    Bisim_SPrintGlobal((char*) &sprint_buf, &snapshot);
    // BsDisplay_Draw(&snapshot);
    Osd_Begin();
    Osd_Ruler();
    Osd_Show();
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

    // rules->game_speed = 0.25f;

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
    Qol_LogInit();
    Qol_UnlockAll();
    Qol_SetCompetitivePrefs();
    // ReplayCard_Init();
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
    //         gm_SetupRulesDefaults(rules);
    // initTestMatch(&vs_mode_data);
}

// static void prepMatch(GameModeState* state)
// {
//     StartMeleeData* start = gm_GetGameModeStateEnterData(state);
//     size_t i;

//     {
//         StartMeleeRules* rules = &start->rules;
//         gm_SetupRulesDefaults(rules);
//         rules->stkind = St_Kind_Last;
//         rules->xB = -1;
//         rules->xC = -1;
//         rules->timer_enabled = true;
//         rules->time_limit = BISIM_MAX_SECONDS;
//         rules->match_kind = MatchKind_Stock;
//         rules->game_speed = 0.25f;

//         /// @todo Handle via gobj
//         rules->on_match_start = onMatchStartRecordVs;
//         /// @todo Handle via replay gobj, ::fighter_alloc_data
// rules->on_frame_end = onFrameEndRecordVs;
//     }

//     for (i = 0; i < Gm_Player_NumMax; i++) {
//         PlayerInitData* player = &start->players[i];
//         gm_SetupPlayerDefaults(player);

//         if (i < ARRAY_SIZE(fighter_init)) {
//             ReplayFighterDesc const* fighter = &fighter_init[i];
//             player->ckind = fighter->ckind;
//             player->color = fighter->color;
//             player->slot = fighter->slot;
//             player->spawn_pos = fighter->spawn_pos;
//             player->slot_type =
//                 fighter->is_cpu ? Gm_PKind_Cpu : Gm_PKind_Human;
//             player->cpu_level = 9;
//             player->damage = 100;
//             player->stocks = 1;
//         } else if (i < PAD_MAX_CONTROLLERS) {
//             player->cpu_kind = CpuKind_4;
//             player->slot_type = Gm_PKind_NA;
//             player->rumble_enabled = false;
//         }
//     }

//     clearReplay();
//     setSeed(BISIM_SEED);
//     gm_LoadAnnouncer();
//     gm_SetupSubColors(start);
// }

void onEnterRecordVs(GameModeState* state)
{
    gmVsMelee_EnterVs(state, &vs_mode_data, onRecordVsStartMelee, NULL);
}

void onEnterPlaybackVs(GameModeState* state) {}

void onExitRecordVs(UNUSED GameModeState* state) {}
void onExitPlaybackVs(UNUSED GameModeState* state) {}

void onExitRecordOver(GameModeState* state) {}
