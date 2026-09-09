#include "replay.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include <fray/lb/lbqol.h>
#include "rpvsmode.h"
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <dolphin/types.h>
#include <melee/ft/types.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/objalloc.h>

static HSD_ObjAllocData frames_alloc_data;
static HSD_GObj* replay_gobj;

void Replay_Init(void)
{
    HSD_ObjAllocInit(&frames_alloc_data,
                     sizeof(ReplayFrame) * REPLAY_MAX_FRAMES, 4);
}

// static void removeUserData(void* user_data)
// {
//     Replayer* rp = user_data;
//     size_t i;

//     for (i = 0; i < Gm_Player_NumMax; i++) {
//         ReplayFrame** frames = &rp->fighters[i].frames;
//         if (frames) {
//             HSD_ObjFree(&frames_alloc_data, rp->fighters[i].frames);
//         }
//     }
//     HSD_Free(rp);
// }

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

// static void recordProc(HSD_GObj* gobj)
// {
//     Replayer* rp = gobj->user_data;
//     size_t i;

//     if (rp->state != ReplayState_Recording) {
//         return;
//     }

//     FRAY_ASSERT(rp->num_frames++ == gm_GetFrameCount());
//     for (i = 0; i < Gm_Player_NumMax; i++) {
//         ReplayFighter* rf = &rp->fighters[i];
//         HSD_GObj* fighter_gobj = Player_GetEntity(i);
//         Fighter* fp = fighter_gobj->user_data;
//         ReplayFrame* rm = Replay_GetCurrentFrame(i);

//         FRAY_ASSERTMSG(rf->pkind != Gm_PKind_Cpu,
//                        "Human recording not implemented!");
//         inputsSetButtons(&rm->in, fp->cpu.buttons);
//         rm->in.lstick = fp->cpu.lstick;
//         rm->in.cstick = fp->cpu.cstick;
//         rm->in.trigger = Replay_GetCpuTrigger(&fp->cpu);
//         rm->out.facing_left = fp->facing_dir < 0.0f;
//         rm->out.airborne = fp->ground_or_air == GA_Air;
//         rm->out.ecb_locked = fp->ecb_lock != 0;
//         rm->out.hit_this_frame = fp->dmg.x18ac_time_since_hit == 0;
//     }
// }

static void renderFunc(UNUSED HSD_GObj* gobj, UNUSED int code)
{
    /// @todo Pass gobj
    // ReplayText_Update();
}

HSD_GObj* Replay_Create(ReplayDesc const* desc)
{
    HSD_GObj* gobj = GObj_Create(REPLAY_GOBJ_CLASS, 1, 0x80);
    // Replayer* rp = HSD_MemAlloc(sizeof(*rp));
    // size_t frames_size = sizeof(ReplayFrame) * desc->num_frames;
    // size_t i;

    // /// @todo Figure out plink and prio
    // GObj_InitUserData(gobj, REPLAY_GOBJ_CLASS, removeUserData, rp);
    // HSD_GObj_SetupProc(gobj, recordProc, 4);
    // // GObj_SetupGXLinkMax(gobj, renderFunc, 0);

    // rp->stkind = desc->stkind;
    // rp->mkind = desc->mkind;
    // rp->version = desc->version;
    // rp->state = desc->state;
    // rp->seed = desc->seed;
    // rp->num_frames = desc->num_frames;

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

HSD_GObj* Replay_GetGObj(void)
{
    FRAY_ASSERT(replay_gobj);
    return replay_gobj;
}

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
