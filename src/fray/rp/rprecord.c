#include "rprecord.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "melee/gm/gm_1A3F.h"
#include "melee/lb/forward.h"
#include "replay.h"
#include "rpcard.h"
#include "rpdisplay.h"
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

enum {
    state_record_vs,
    state_playback_vs,
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
    {
        state_playback_vs,
        lbDvdPreload_2,
        0,
        onEnterPlaybackVs,
        onExitPlaybackVs,
        {
            GS_VS,
            &gmVsMelee_StartData,
            &gmVsMelee_VsExitInfo,
        },
    },
    {
        GM_GAMEMODESTATE_TERMINATE,
        lbDvdPreload_0,
        0,
        NULL,
        NULL,
        { 0 },
    },
};

void Replay_Mode_OnInit(void)
{
    Qol_LogInit();
    Replay_Init();
    ReplayCard_Init();
}

void Replay_Mode_OnLoad(void)
{
    Replay_Load();
}

void Replay_Mode_OnUnload(void) {}

static void setSeed(u32 seed)
{
    *seed_ptr = seed;
}

static void onMatchStartRecordVs(void)
{
    ReplayText_Setup();
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
//         rules->time_limit = REPLAY_MAX_SECONDS;
//         rules->match_kind = MatchKind_Stock;
//         rules->game_speed = 0.25f;

//         /// @todo Handle via gobj
//         rules->on_match_start = onMatchStartRecordVs;
//         /// @todo Handle via replay gobj, ::fighter_alloc_data
//         // rules->on_frame_end = onFrameEndRecordVs;
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
//     setSeed(REPLAY_SEED);
//     gm_LoadAnnouncer();
//     gm_SetupSubColors(start);
// }

void onEnterRecordVs(GameModeState* state)
{
#if 0
    StartMeleeData* start = gm_GetGameModeStateEnterData(state);
    Replay* rp = Replay_GetGObj()->user_data;
    size_t i;

    {
        StartMeleeRules* rules = &start->rules;
        gm_SetupRulesDefaults(rules);
        rules->stkind = St_Kind_Last;
        rules->xB = -1;
        rules->xC = -1;
        rules->timer_enabled = true;
        rules->time_limit = REPLAY_MAX_SECONDS;
        rules->match_kind = MatchKind_Stock;
        rules->game_speed = 0.25f;
        rules->on_match_start = onMatchStartRecordVs;
        // rules->on_frame_end = onFrameEndRecordVs;
    }

    for (i = 0; i < Gm_Player_NumMax; i++) {
        PlayerInitData* player = &start->players[i];
        ReplayFighter* fighter = &rp->fighters[i];
        gm_SetupPlayerDefaults(player);

        //     if (i < ARRAY_SIZE(fighter_init)) {
        //         ReplayFighterDesc const* fighter = &fighter_init[i];
        player->ckind = fighter->ckind;
        player->color = fighter->color;
        player->slot = fighter->slot;
        player->spawn_pos = fighter->spawn_pos;
        player->slot_type = fighter->pkind;
        player->cpu_level = 9;
        player->damage = 100;
        player->stocks = 1;
        //     } else if (i < PAD_MAX_CONTROLLERS) {
        //         player->cpu_kind = CpuKind_4;
        //         player->slot_type = Gm_PKind_NA;
        //         player->rumble_enabled = false;
        //     }
    }

    // clearReplay();
    // setSeed(REPLAY_SEED);
#endif
    gm_LoadAnnouncer();
    // gm_SetupSubColors(start);
}

void onEnterPlaybackVs(GameModeState* state) {}

void onExitRecordVs(UNUSED GameModeState* state) {}
void onExitPlaybackVs(UNUSED GameModeState* state) {}
