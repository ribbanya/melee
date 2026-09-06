#include <dolphin/os.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>

static void onEnterRecordVs(GameModeState* state);

enum {
    state_record_vs,
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
    { GM_GAMEMODESTATE_TERMINATE },
};

void Replay_Mode_OnInit(void) {}

void Replay_Mode_OnLoad(void) {}

void Replay_Mode_OnUnload(void) {}

void onEnterRecordVs(GameModeState* state)
{
    StartMeleeData* start = gm_GetGameModeStateEnterData(state);
    ssize_t i;

    {
        StartMeleeRules* rules = &start->rules;
        gm_SetupRulesDefaults(rules);
        rules->stkind = St_Kind_Last;
        rules->xB = -1;
        rules->xC = -1;
        rules->timer_enabled = true;
        rules->time_limit = 10;
        rules->match_kind = MatchKind_Stock;
        rules->game_speed = 2.0f;
    }

    for (i = 0; i < Gm_Player_NumMax; i++) {
        PlayerInitData* player = &start->players[i];
        gm_SetupPlayerDefaults(player);

        if (i < ARRAY_SIZE(fighters)) {
            FighterSetup const* fighter = &fighters[i];
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

    gm_LoadAnnouncer();
}
