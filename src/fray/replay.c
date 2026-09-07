#include <abort_exit.h> // IWYU pragma: keep

#include "melee/gm/forward.h"
#include "melee/pl/forward.h"
#include "melee/pl/player.h"
#include "Runtime/platform.h"
#include <dolphin/os.h>
#include <fray/replaycard.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
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

void Replay_Mode_OnInit(void)
{
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
    FighterSetup const* fs;
    int slot;
    HSD_GObj* gobj;
    Fighter* fp;
    HSD_Pad buttons;
    Gm_PKind pkind;
    Vec2 lstick;
    Vec2 cstick;
    size_t i;

    for (i = 0; i < 6; i++) {
        OSReport("%d: %d", i, Player_GetPlayerSlotType(i));
    }
    return;

    for (i = 0; i < ARRAY_SIZE(fighters); i++) {
        fs = &fighters[i];
        slot = fs->slot;

        gobj = Player_GetEntity(slot);
        if (!gobj) {
            continue;
        }

        fp = gobj->user_data;
        if (!fp) {
            continue;
        }

        buttons = 0;
        pkind = Player_GetPlayerSlotType(slot);
        switch (pkind) {
        case Gm_PKind_Human:
            buttons = fp->input.held_inputs;
            lstick = fp->input.lstick;
            cstick = fp->input.cstick;
            break;
        case Gm_PKind_Cpu:
            buttons = fp->x1A88.x0;
            lstick.x = fp->x1A88.lstickX / (float) S8_MAX;
            lstick.y = fp->x1A88.lstickY / (float) S8_MAX;
            cstick.x = fp->x1A88.cstickX / (float) S8_MAX;
            cstick.y = fp->x1A88.cstickY / (float) S8_MAX;
            break;
        case Gm_PKind_Demo:
        case Gm_PKind_NA:
        case Gm_PKind_Boss:
            OSPanic(__FILE__, __LINE__, "Unexpected PKind %d!", pkind);
        }

        if (buttons) {
            OSReport("%d: (%.2f,%.2f) (%.2f,%.2f) %08x", i, lstick.x, lstick.y,
                     cstick.x, cstick.y, buttons);
        }
    }
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
        rules->on_frame_end = recordInputs;
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
