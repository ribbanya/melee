#include "rprecord.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/pl/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "melee/ft/types.h"
#include "melee/gm/gm_16AE.h"
#include "melee/gm/gm_1B03.h"
#include "melee/lb/lb_00B0.h"
#include "melee/pl/player.h"
#include "rpcard.h"
#include "rpdisplay.h"
#include "sysdolphin/baselib/controller.h"
#include <dolphin/types.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <sysdolphin/baselib/random.h>

static void onEnterRecordVs(GameModeState* state);
static void onExitRecordVs(GameModeState* state);

static u32 const fixed_seed = 0xDEEDBEEF;

static ReplayFighterInit const fighter_init[] = {
    { CKIND_FOX, Gm_PKind_Cpu, 0, 0, 2 },
    { CKIND_FOX, Gm_PKind_Cpu, 2, 3, 3 },
};

static ReplayFighter fighter_replay[ARRAY_SIZE(fighter_init)];

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

static void checkSlot(int slot)
{
    if (slot < 0 || !(slot < ARRAY_SIZE(fighter_replay))) {
        HSD_ASSERTREPORT(__LINE__, 0, "Slot out of bounds! %d\n", slot);
    }
}

static void checkFrame(u32 frame)
{
    if (!(frame < REPLAY_MAX_FRAMES)) {
        HSD_ASSERTREPORT(__LINE__, 0, "Frame out of bounds! %d\n", frame);
    }
}

ReplayFrame* Replay_GetCurrentFrame(int slot)
{
    u32 frame = gm_GetFrameCount();
    checkSlot(slot);
    checkFrame(frame);
    return &fighter_replay[slot].frames[frame];
}

void Replay_Mode_OnLoad(void) {}

void Replay_Mode_OnUnload(void) {}

static void resetSeed(void)
{
    *seed_ptr = fixed_seed;
}

static void resetReplay(void)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(fighter_replay); i++) {
        fighter_replay[i].init = fighter_init[i];
        memzero(fighter_replay[i].frames, sizeof(fighter_replay[i].frames));
    }
}

static void onMatchStartRecordVs(void)
{
    ReplayText_Setup();
}

s8 Replay_GetCpuTrigger(struct CpuFighter* cpu)
{
    return MAX(cpu->ltrigger, cpu->rtrigger);
}

static void rpFrameSetButtons(ReplayFrame* rf, HSD_Pad buttons)
{
    rf->a = (buttons & HSD_PAD_A) != 0;
    rf->b = (buttons & HSD_PAD_B) != 0;
    rf->x = (buttons & HSD_PAD_X) != 0;
    rf->y = (buttons & HSD_PAD_Y) != 0;
    rf->l = (buttons & HSD_PAD_L) != 0;
    rf->r = (buttons & HSD_PAD_R) != 0;
    rf->z = (buttons & HSD_PAD_Z) != 0;
    rf->dpad_up = (buttons & HSD_PAD_DPADUP) != 0;
}

static void recordFrame(void)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(fighter_replay); i++) {
        ReplayFighter* rp = &fighter_replay[i];
        HSD_GObj* gobj = Player_GetEntity(i);
        Fighter* fp = gobj->user_data;
        ReplayFrame* rf = Replay_GetCurrentFrame(i);

        HSD_ASSERTMSG(__LINE__, rp->init.is_cpu,
                      "Human recording not implemented!");
        rpFrameSetButtons(rf, fp->cpu.buttons);
        rf->lstick = fp->cpu.lstick;
        rf->cstick = fp->cpu.cstick;
        rf->trigger = Replay_GetCpuTrigger(&fp->cpu);
        rf->facing_left = fp->facing_dir < 0.0f;
        rf->airborne = fp->ground_or_air == GA_Air;
        rf->ecb_locked = fp->ecb_lock != 0;
        rf->hit_this_frame = fp->dmg.x18ac_time_since_hit == 0;
    }
}

static void onFrameEndRecordVs(void)
{
    recordFrame();
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
        rules->timer_enabled = true;
        rules->time_limit = REPLAY_MAX_SECONDS;
        rules->match_kind = MatchKind_Stock;
        rules->game_speed = 0.25f;
        rules->on_match_start = onMatchStartRecordVs;
        rules->on_frame_end = onFrameEndRecordVs;
    }

    for (i = 0; i < Gm_Player_NumMax; i++) {
        PlayerInitData* player = &start->players[i];
        gm_SetupPlayerDefaults(player);

        if (i < ARRAY_SIZE(fighter_init)) {
            ReplayFighterInit const* fighter = &fighter_init[i];
            player->ckind = fighter->ckind;
            player->color = fighter->color;
            player->slot = fighter->slot;
            player->x5 = fighter->spawn_pos;
            player->slot_type =
                fighter->is_cpu ? Gm_PKind_Cpu : Gm_PKind_Human;
            player->cpu_level = 9;
            player->x10 = 100;
            player->stocks = 1;
        } else if (i < PAD_MAX_CONTROLLERS) {
            player->cpu_kind = CpuKind_4;
            player->slot_type = Gm_PKind_NA;
            player->rumble_enabled = false;
        }
    }

    resetSeed();
    resetReplay();
    gm_LoadAnnouncer();
    gm_SetupSubColors(start);
}

void onExitRecordVs(UNUSED GameModeState* state) {}
