#include "replay.h"

#include <string.h>

#include "melee/gm/forward.h"
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjuserdata.h>
#include <sysdolphin/baselib/memory.h>
#include <sysdolphin/baselib/objalloc.h>

static HSD_ObjAllocData frames_alloc_data;
static HSD_GObj* replay_gobj;

static Replay const replay_desc = {
    St_Kind_Last,
    MatchKind_Stock,
    ReplayVersion_Current,
    false,
    0xFEEDBEEF,
    0,
    {
        { CKIND_FOX, Gm_PKind_Cpu, CpuKind_4, 0, 0, 3, NULL },
        { CKIND_FOX, Gm_PKind_Cpu, CpuKind_4, 1, 0, 1, NULL },
    },
};

void Replay_Init(void)
{
    HSD_ObjAllocInit(&frames_alloc_data,
                     sizeof(ReplayFrame) * REPLAY_MAX_FRAMES, 4);
}

static void removeUserData(void* user_data)
{
    Replay* rp = user_data;
    size_t i;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        ReplayFrame** frames = &rp->fighters[i].frames;
        if (frames) {
            HSD_ObjFree(&frames_alloc_data, rp->fighters[i].frames);
        }
    }
    HSD_Free(rp);
}

static HSD_GObj* createReplayGObj(Replay const* desc)
{
    HSD_GObj* gobj = GObj_Create(REPLAY_GOBJ_CLASS, 1, 0x80);
    Replay* rp = HSD_MemAlloc(sizeof(*rp));
    size_t frames_size = sizeof(ReplayFrame) * desc->num_frames;
    size_t i;

    /// @todo Figure out plink and prio
    GObj_InitUserData(gobj, REPLAY_GOBJ_CLASS, removeUserData, rp);

    rp->stkind = desc->stkind;
    rp->mkind = desc->mkind;
    rp->version = desc->version;
    rp->seed = desc->seed;
    rp->num_frames = desc->num_frames;

    for (i = 0; i < Gm_Player_NumMax; i++) {
        ReplayFighter const* src = &desc->fighters[i];
        ReplayFighter* dst = &rp->fighters[i];

        dst->ckind = src->ckind;
        dst->pkind = src->ckind;
        dst->cpu_kind = src->cpu_kind;
        dst->color = src->color;
        dst->slot = src->slot;
        dst->spawn_pos = src->spawn_pos;
        dst->frames = HSD_ObjAlloc(&frames_alloc_data);
        memcpy(dst->frames, src->frames, frames_size);
    }

    return gobj;
};

HSD_GObj* Replay_GetOrCreateGObj(void)
{
    if (replay_gobj) {
        return replay_gobj;
    }
    return createReplayGObj(&replay_desc);
}
