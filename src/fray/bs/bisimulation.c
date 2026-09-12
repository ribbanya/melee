#include "bisimulation.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "bsvsmode.h"
#include "melee/ft/forward.h"
#include "melee/ft/kinds/ftCommon/ftCo_0A01.h"
#include "melee/gm/forward.h"
#include "melee/lb/lb_00B0.h"
#include "melee/pl/forward.h"
#include "melee/pl/player.h"
#include "sysdolphin/baselib/gobjuserdata.h"
#include "sysdolphin/baselib/memory.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/ft/types.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjproc.h>
#include <sysdolphin/baselib/objalloc.h>

void Replay_Init(void) {}

u32 Bisim_HashFighter(HSD_GObj* gobj) {}

u32 Bisim_HashAllFighters(u32 h)
{
    u32 h;
}

u32 Bisim_HashGameState(void)
{
    u32 h;

    h = Bisim_HashAllFighters();

    return h;
}

static void renderFunc(UNUSED HSD_GObj* gobj, UNUSED int code)
{
    /// @todo Pass gobj
    // ReplayText_Update();
}

HSD_GObj* Replay_Create(ReplayDesc const* desc)
{
    HSD_GObj* gobj = GObj_Create(BISIM_CLASS, BISIM_PLINK, 0);
    Replayer* rp = HSD_MemAlloc(sizeof(*rp));
    size_t frames_size = sizeof(ReplayFrame) * desc->num_frames;
    size_t i;

    GObj_InitUserData(gobj, BISIM_USER_DATA_KIND, removeUserData, rp);

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
