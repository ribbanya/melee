#include "bisimulation.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "fray/bs/bshash.h"
#include "fray/bs/bsio.h"
#include "melee/ft/forward.h"
#include "melee/gm/gmvs.h"
#include <dolphin/types.h>
#include <melee/ft/types.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjproc.h>

void Bisim_CaptureFighter(Bisim_FighterSnapshot* dst, const Fighter* src)
{
    dst->msid = src->motion_id;
    dst->anim_frame = src->cur_anim_frame;
    dst->airborne = src->ground_or_air;
    dst->pos = src->cur_pos;
    dst->accel = src->x74_self_accel;
    dst->vel = src->self_vel;
    dst->kb_vel = src->x8c_kb_vel;
}

void Bisim_WriteFighter(BsIO_Cursor* c, const Bisim_FighterSnapshot* fp) {}

u32 Bisim_HashFighter(u32 h, HSD_GObj* gobj)
{
    Fighter* fp = gobj->user_data;
    h = bsHash_Vec3(h, &fp->cur_pos);
    h = bsHash_Vec3(h, &fp->self_vel);
    h = bsHash_Vec3(h, &fp->x8c_kb_vel);
    return h;
}

u32 Bisim_HashAllFighters(u32 h)
{
    HSD_GObj* cur;
    for (cur = HSD_GObjPLinkHead[HSD_GOBJ_PLINK_FIGHTER]; cur != NULL;
         cur = HSD_GObjGetNext(cur))
    {
        h = Bisim_HashFighter(h, cur);
    }
    return h;
}

u32 Bisim_HashGameState(u32 h)
{
    h = bsHash_U32(h, gm_GetFrameCount());
    h = Bisim_HashAllFighters(h);
    return h;
}
