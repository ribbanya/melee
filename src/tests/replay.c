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
        1,
        lbDvdPreload_2,
        0,
        NULL,
        NULL,
        {
            GS_UNK10,
            NULL,
            NULL,
        },
    },
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

void Replay_Mode_OnInit(void)
{
    OSReport("Welcome to %s!", __FILE__);
}

void Replay_Mode_OnLoad(void) {}
void Replay_Mode_OnUnload(void) {}

void onEnterRecordVs(GameModeState* state)
{
    StartMeleeData* start = gm_GetGameModeStateEnterData(state);
    ssize_t i;

    gm_SetupRulesDefaults(&start->rules);
    start->rules.stkind = St_Kind_Last;
    start->rules.xB = -1;
    start->rules.xC = -1;
    start->rules.match_kind = MatchKind_Time;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        gm_SetupPlayerDefaults(&start->players[i]);
        start->players[i].stocks = 0;
        start->players[i].cpu_kind = 4;
    }

    start->players[0].ckind = CKIND_LINK;
    start->players[1].ckind = CKIND_MARIO;
    start->players[2].ckind = CKIND_LINK;
    start->players[3].ckind = CKIND_LINK;

    start->players[0].slot_type = Gm_PKind_Human;
    start->players[1].slot_type = Gm_PKind_Human;
    start->players[2].slot_type = Gm_PKind_NA;
    start->players[3].slot_type = Gm_PKind_NA;

    start->players[0].rumble_enabled = false;
    start->players[1].rumble_enabled = false;
    start->players[2].rumble_enabled = false;
    start->players[3].rumble_enabled = false;

    gm_LoadAnnouncer();
}
