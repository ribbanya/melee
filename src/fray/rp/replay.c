#include "replay.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "melee/ft/forward.h"
#include "melee/ft/kinds/ftCommon/ftCo_0A01.h"
#include "melee/gm/forward.h"
#include "melee/lb/lb_00B0.h"
#include "melee/pl/forward.h"
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

void Replay_Init(void) {}

static void removeUserData(void* user_data)
{
    Replayer* rp = user_data;
    size_t i;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        ReplayFrame* frames = rp->frames[i];
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

static void recordInputs(Fighter* fp, ReplayFrame* rf)
{
    if (ftCo_IsCpuControlled(fp)) {
        inputsSetButtons(&rf->in, fp->cpu.buttons);
        rf->in.lstick = fp->cpu.lstick;
        rf->in.cstick = fp->cpu.cstick;
        rf->in.trigger = Replay_GetCpuTrigger(&fp->cpu);
        rf->out.facing_left = fp->facing_dir < 0.0f;
        rf->out.airborne = fp->ground_or_air == GA_Air;
        rf->out.ecb_locked = fp->ecb_lock != 0;
        rf->out.hit_this_frame = fp->dmg.x18ac_time_since_hit == 0;
    } else {
        FRAY_ASSERTMSG(false, "Human recording not implemented!");
    }
}

static void checkWeirdNana(int slot, int sub)
{
    StaticPlayer* pp;
    HSD_GObj* gobj;
    Fighter* fp;
    u8 transformed;

    pp = Player_GetPtrForSlot(slot);
    FRAY_ASSERT(pp);

    transformed = pp->transformed[sub];

    gobj = pp->player_entity[pp->transformed[sub]];
    FRAY_ASSERT(gobj);

    fp = gobj->user_data;
    FRAY_ASSERT(fp);

    if (fp->kind != Ft_Kind_Nana) {
        return;
    }
    FRAY_ASSERT(pp->ckind == CKind_PopoNana);
    FRAY_ASSERT(sub == 1);
    FRAY_ASSERT(transformed == 1);
    FRAY_ASSERT(ftCo_IsCpuControlled(fp));
    FRAY_ASSERT(pp->cpu_type == CpuKind_4);
    FRAY_ASSERT(fp->is_sub_fighter);
    FRAY_ASSERT(fp->cpu.xF8_b0);
}

static void recordProc(HSD_GObj* gobj)
{
    Replayer* rp = gobj->user_data;
    HSD_GObj* cur;

    if (rp->state != ReplayState_Recording) {
        return;
    }

    // FRAY_ASSERT(rp->num_frames++ == gm_GetFrameCount());

    for (cur = HSD_GObjPLinkHead[HSD_GOBJ_PLINK_FIGHTER]; cur != NULL;
         cur = HSD_GObjGetNext(cur))
    {
        Fighter* fp = cur->user_data;
        // Fighter* fp = pp->player_entity[pp->transformed[j]]->user_data;
        // ReplayFrame* rf = rp->frames[i];
        if (fp != NULL) {
            size_t i = fp->player_id;
            size_t j = fp->is_sub_fighter;
            StaticPlayer* pp = Player_GetPtrForSlot(i);
            bool is_cpu;
            OSReport("\n=== %d[%d] ===\n", i, j);
            REPORT_INT(is_cpu = ftCo_IsCpuControlled(fp));
            REPORT_INT(fp->kind);
            REPORT_INT(pp->pkind);
            REPORT_INT(pp->ckind);
            REPORT_INT(pp->cpu_type);
            REPORT_INT(fp->cpu.kind);
            REPORT_INT(fp->is_sub_fighter);
            REPORT_INT(fp->cpu.xF8_b0);

            REPORT_ADDR(fp);
            if (is_cpu) {
                REPORT_HEX(fp->cpu.buttons);
            } else {
                REPORT_HEX(HSD_PadGameStatus[fp->x618_player_id].button);
            }
            checkWeirdNana(i, j);
            // recordInputs(fp, rf, pp->slot_type);
        }
    }
}

static void renderFunc(UNUSED HSD_GObj* gobj, UNUSED int code)
{
    /// @todo Pass gobj
    // ReplayText_Update();
}

HSD_GObj* Replay_Create(ReplayDesc const* desc)
{
    HSD_GObj* gobj = GObj_Create(REPLAY_CLASS, REPLAY_PLINK, 0);
    Replayer* rp = HSD_MemAlloc(sizeof(*rp));
    size_t frames_size = sizeof(ReplayFrame) * desc->num_frames;
    size_t i;

    GObj_InitUserData(gobj, REPLAY_USER_DATA_KIND, removeUserData, rp);

    /// @todo Extract fighter proc prios to header
    HSD_GObj_SetupProc(gobj, recordProc, 3);

    REPORT_HEX(rp->num_frames = desc->num_frames);

    for (i = 0; i < Gm_Player_NumMax; i++) {
        if (desc->enabled_slots & (1 << i)) {
            rp->frames[i] = HSD_MemAlloc(frames_size);
        }
    }

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
