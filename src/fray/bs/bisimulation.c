#include "bisimulation.h"

#include <Runtime/platform.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "fray/bs/bshash.h"
#include "fray/bs/bsio.h"
#include "melee/ft/forward.h"
#include "melee/gm/gmvs.h"
#include "melee/pl/player.h"
#include "sysdolphin/baselib/random.h"
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

void Bisim_CapturePlayer(Bisim_PlayerSnapshot* dst, const StaticPlayer* src)
{
    size_t i;

    dst->pkind = src->pkind;
    dst->color = src->costume_id;
    dst->port = src->controller_index;

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Bisim_CaptureFighter(&dst->sub_fighters[i],
                             src->player_entity[i]->user_data);
    }
}

void Bisim_CaptureGlobal(Bisim_GlobalSnapshot* dst)
{
    size_t i;

    dst->seed = *seed_ptr;
    dst->curr_frame = gm_GetFrameCount();

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Bisim_CapturePlayer(&dst->players[i], Player_GetPtrForSlot(i));
    }
}

void Bisim_WriteFighter(BsIO_Cursor* c, const Bisim_FighterSnapshot* v)
{
    bsIO_WriteS32(c, v->msid);
    bsIO_WriteF32(c, v->anim_frame);
    bsIO_WriteBool(c, v->airborne);
    bsIO_WriteVec3(c, &v->pos);
    bsIO_WriteVec3(c, &v->accel);
    bsIO_WriteVec3(c, &v->vel);
    bsIO_WriteVec3(c, &v->kb_vel);
}

void Bisim_WritePlayer(BsIO_Cursor* c, const Bisim_PlayerSnapshot* v)
{
    size_t i;

    bsIO_WriteU8(c, v->pkind);
    bsIO_WriteU8(c, v->color);
    bsIO_WriteU8(c, v->port);

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Bisim_WriteFighter(c, &v->sub_fighters[i]);
    }
}

void Bisim_WriteGlobal(BsIO_Cursor* c, const Bisim_FighterSnapshot* fp) {}

void Bisim_ReadFighter(BsIO_Cursor* c, Bisim_FighterSnapshot* v)
{
    v->msid = bsIO_ReadS32(c);
    v->anim_frame = bsIO_ReadF32(c);
    v->airborne = bsIO_ReadBool(c);
    bsIO_ReadVec3(c, &v->pos);
    bsIO_ReadVec3(c, &v->accel);
    bsIO_ReadVec3(c, &v->vel);
    bsIO_ReadVec3(c, &v->kb_vel);
}
