#include "replay.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/pl/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "replaycard.h"
#include <fray/replaytext.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <sysdolphin/baselib/random.h>

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);

static u32 const fixed_seed = 0xDEADBEEF;

static ReplayFighter fighters[] = {
    { CKIND_FOX, 0, 0, 2 },
    { CKIND_FOX, 2, 3, 4 },
};

enum {
    state_record_vs,
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
    { GM_GAMEMODESTATE_TERMINATE },
};

static int shut_up(UNUSED __file_handle arg0, UNUSED unsigned char* arg1,
                   UNUSED size_t* arg2, UNUSED __idle_proc arg3)
{
    return 0;
}

void Replay_Mode_OnInit(void)
{
    // Shut up character-by-character OSReport spam. Will crash if null.
    stdout->write_proc = shut_up;
    ReplayCard_Init();
}

void Replay_Mode_OnLoad(void) {}

void Replay_Mode_OnUnload(void) {}

static void resetSeed(void)
{
    *seed_ptr = fixed_seed;
}

static void onMatchStartRecordVs(void)
{
    ReplayText_Setup();
}

static void onFrameEndRecordVs(void)
{
    ReplayText_Update();
}

void onEnterRecordVs(GameModeState* state)
{
    StartMeleeData* start = gm_GetGameModeStateEnterData(state);
    size_t i;

    {
        StartMeleeRules* rules = &start->rules;
        gm_SetupRulesDefaults(rules);
        rules->stkind = St_Kind_Last;
        rules->xB = -1;
        rules->xC = -1;
        rules->timer_enabled = false;
        rules->match_kind = MatchKind_Stock;
        rules->game_speed = 0.25f;
        rules->on_match_start = onMatchStartRecordVs;
        rules->on_frame_end = onFrameEndRecordVs;
    }

    for (i = 0; i < Gm_Player_NumMax; i++) {
        PlayerInitData* player = &start->players[i];
        gm_SetupPlayerDefaults(player);

        if (i < ARRAY_SIZE(fighters)) {
            ReplayFighter const* fighter = &fighters[i];
            player->ckind = fighter->ckind;
            player->color = fighter->color;
            player->slot = fighter->slot;
            player->x5 = fighter->spawn_pos;
            player->slot_type = Gm_PKind_Cpu;
            player->cpu_level = 9;
            player->x10 = 300;
            player->stocks = 1;
        } else if (i < PAD_MAX_CONTROLLERS) {
            player->cpu_kind = CpuKind_4;
            player->slot_type = Gm_PKind_NA;
            player->rumble_enabled = false;
        }
    }

    resetSeed();
    gm_LoadAnnouncer();
}

void onExitRecordVs(UNUSED GameModeState* state) {}
