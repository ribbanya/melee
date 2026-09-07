#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/pl/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include <dolphin/os.h>
#include <fray/replaycard.h>
#include <fray/replaytext.h>
#include <melee/ft/kinds/ftCommon/ftCo_0A01.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <melee/pl/player.h>
#include <sysdolphin/baselib/random.h>

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);

static u32 const fixed_seed = 0xDEADBEEF;

typedef struct {
    CharacterKind ckind;
    int color;
    int slot;
    int spawn_pos;
} FighterSetup;

static FighterSetup const fighters[] = {
    { CKIND_FOX, 3, 2, 2 },
    { CKIND_FOX, 3, 0, 4 },
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
    // Shut up character-by-character OSReport spam
    // Can't be null or it will just get replaced in ::HSD_LogInit
    stdout->write_proc = shut_up;
    ReplayCard_Init();
}

void Replay_Mode_OnLoad(void) {}

void Replay_Mode_OnUnload(void) {}

static void resetSeed(void)
{
    *seed_ptr = fixed_seed;
}

static void recordInputs(void)
{
    HSD_GObj* gobj;
    Fighter* fp;
    HSD_Pad buttons;
    Gm_PKind pkind;
    Vec2 lstick;
    Vec2 cstick;
    size_t i;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        pkind = Player_GetPlayerSlotType(i);

        if (pkind == Gm_PKind_NA) {
            continue;
        }

        gobj = Player_GetEntity(i);
        if (!gobj) {
            OSReport("Can't get player %d!", i);
            continue;
        }

        fp = gobj->user_data;
        if (!fp) {
            OSReport("Can't get fighter %d!", i);
            continue;
        }

        buttons = 0;
        switch (pkind) {
        case Gm_PKind_Human:
            buttons = fp->input.held_inputs;
            lstick = fp->input.lstick;
            cstick = fp->input.cstick;
            break;
        case Gm_PKind_Cpu:
            buttons = ftCo_GetCpuButtons(fp);
            lstick.x = ftCo_GetCpuLStickX(fp);
            lstick.y = ftCo_GetCpuLStickY(fp);
            cstick.x = ftCo_GetCpuCStickX(fp);
            cstick.y = ftCo_GetCpuCStickY(fp);
            break;
        case Gm_PKind_NA:
        case Gm_PKind_Demo:
        case Gm_PKind_Boss:
            OSPanic(__FILE__, __LINE__, "Unexpected PKind %d!", pkind);
        }

        if (buttons) {
            OSReport("%d: (%.2f,%.2f) (%.2f,%.2f) %08x", i, lstick.x, lstick.y,
                     cstick.x, cstick.y, buttons);
        }
    }
}

static void onMatchStartRecordVs(void)
{
    ReplayText_Setup();
}

static void onFrameEndRecordVs(void)
{
    // recordInputs();
    ReplayText_Update();
    // DevText_ShowBackground(db_CpuHandicapInfo.text);
    // DevText_ShowText(db_CpuHandicapInfo.text);
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
        rules->timer_enabled = true;
        rules->time_limit = 10;
        rules->match_kind = MatchKind_Stock;
        rules->game_speed = 2.0f;
        rules->on_match_start = onMatchStartRecordVs;
        rules->on_frame_end = onFrameEndRecordVs;
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

    resetSeed();
    gm_LoadAnnouncer();
}

void onExitRecordVs(UNUSED GameModeState* state) {}
