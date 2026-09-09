#include "replay.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "melee/pl/player.h"
#include "rpvsmode.h"
#include "sysdolphin/baselib/gobjuserdata.h"
#include "sysdolphin/baselib/memory.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/ft/types.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/objalloc.h>

static HSD_ObjAllocData frames_alloc_data;

void Replay_Init(void)
{
    HSD_ObjAllocInit(&frames_alloc_data,
                     sizeof(ReplayFrame) * REPLAY_MAX_FRAMES, 4);
}

static void removeUserData(void* user_data)
{
    Replayer* rp = user_data;
    size_t i;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        ReplayFrame** frames = &rp->frames[i];
        if (frames != NULL) {
            HSD_ObjFree(&frames_alloc_data, frames);
        }
    }
    HSD_Free(rp);
}

static void inputsSetButtons(ReplayInputs* ri, HSD_Pad buttons)
{
    ri->a = (buttons & HSD_PAD_A) != 0;
    ri->b = (buttons & HSD_PAD_B) != 0;
    ri->x = (buttons & HSD_PAD_X) != 0;
    ri->y = (buttons & HSD_PAD_Y) != 0;
    ri->l = (buttons & HSD_PAD_L) != 0;
    ri->r = (buttons & HSD_PAD_R) != 0;
    ri->z = (buttons & HSD_PAD_Z) != 0;
    ri->dpad_up = (buttons & HSD_PAD_DPADUP) != 0;
}

s8 Replay_GetCpuTrigger(struct CpuFighter* cpu)
{
    return MAX(cpu->ltrigger, cpu->rtrigger);
}

static void recordProc(HSD_GObj* gobj)
{
    Replayer* rp = gobj->user_data;
    size_t i;

    if (rp->state != ReplayState_Recording) {
        return;
    }

    // FRAY_ASSERT(rp->num_frames++ == gm_GetFrameCount());
    for (i = 0; i < Gm_Player_NumMax; i++) {
        ReplayFrame* rf = rp->frames[i];
        StaticPlayer* pp;
        HSD_GObj* fighter_gobj;
        Fighter* fp;
        if (rf == NULL) {
            continue;
        }

        pp = Player_GetPtrForSlot(i);
        fighter_gobj = pp->player_entity[0];

        /// @todo support Sheik
        fp = fighter_gobj->user_data;
        // ReplayFrame* rm = &rm-

        FRAY_ASSERTMSG(pp->slot_type != Gm_PKind_Cpu,
                       "Human recording not implemented!");
        inputsSetButtons(&rf->in, fp->cpu.buttons);
        rf->in.lstick = fp->cpu.lstick;
        rf->in.cstick = fp->cpu.cstick;
        rf->in.trigger = Replay_GetCpuTrigger(&fp->cpu);
        rf->out.facing_left = fp->facing_dir < 0.0f;
        rf->out.airborne = fp->ground_or_air == GA_Air;
        rf->out.ecb_locked = fp->ecb_lock != 0;
        rf->out.hit_this_frame = fp->dmg.x18ac_time_since_hit == 0;
    }
}

static void renderFunc(UNUSED HSD_GObj* gobj, UNUSED int code)
{
    /// @todo Pass gobj
    // ReplayText_Update();
}

HSD_GObj* Replay_Create(ReplayDesc const* desc)
{
    HSD_GObj* gobj =
        GObj_Create(REPLAY_CLASS, REPLAY_PLINK, FRAY_PRIORITY_MAX);
    Replayer* rp = HSD_MemAlloc(sizeof(*rp));
    size_t frames_size = sizeof(ReplayFrame) * desc->num_frames;
    size_t i;

    /// @todo Figure out plink and prio
    GObj_InitUserData(gobj, REPLAY_USER_DATA_KIND, removeUserData, rp);
    HSD_GObj_SetupProc(gobj, recordProc, FRAY_PRIORITY_MAX);
    // GObj_SetupGXLinkMax(gobj, renderFunc, 0);

    rp->num_frames = desc->num_frames;

    // for (i = 0; i < Gm_Player_NumMax; i++) {
    //     ReplayFighter const* src = &desc->fighters[i];
    //     ReplayFighter* dst = &rp->fighters[i];

    //     dst->ckind = src->ckind;
    //     dst->pkind = src->ckind;
    //     dst->cpu_kind = src->cpu_kind;
    //     dst->color = src->color;
    //     dst->slot = src->slot;
    //     dst->spawn_pos = src->spawn_pos;
    //     dst->frames = HSD_ObjAlloc(&frames_alloc_data);
    //     memcpy(dst->frames, src->frames, frames_size);
    // }

    return gobj;
};

static void checkFrame(Replayer* rp, u32 frame)
{
    if (!(frame < rp->num_frames)) {
        FRAY_ASSERTREPORT(0, "Frame out of bounds! %d\n", frame);
    }
}

// ReplayFrame* Replay_GetCurrentFrame(int slot)
// {
//     Replayer* rp = Replay_GetGObj()->user_data;
//     u32 frame = gm_GetFrameCount();
//     checkFrame(rp, frame);
//     return &rp->fighters[slot].frames[frame];
// }
