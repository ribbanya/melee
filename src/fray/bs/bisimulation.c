#include "bisimulation.h"

#include <melee/ft/forward.h>
#include <melee/gm/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "bsio.h"
#include <fray/lb/lbqol.h>
#include <melee/ft/inlines.h>
#include <melee/ft/types.h>
#include <melee/gm/gmvs.h>
#include <melee/pl/player.h>
#include <sysdolphin/baselib/random.h>

void Bisim_CaptureFighter(Bisim_FighterSnapshot* dst, const Fighter* src)
{
    if (src == NULL) {
        memset(dst, 0, sizeof(*dst));
        return;
    }

    dst->exists = true;
    dst->ftkind = src->kind;
    dst->msid = src->motion_id;
    dst->anim_frame = src->cur_anim_frame;
    dst->airborne = src->ground_or_air;
    dst->facing_dir = ftGetFacingDirInt((Fighter*) src);
    dst->pos = src->cur_pos;
    dst->accel = src->x74_self_accel;
    dst->self_vel = src->self_vel;
    dst->kb_vel = src->x8c_kb_vel;
    dst->buttons = src->input.held_buttons[0];
    dst->lstick = src->input.lstick[0];
    dst->cstick = src->input.cstick[0];
}

void Bisim_CapturePlayer(Bisim_PlayerSnapshot* dst, const StaticPlayer* src)
{
    size_t i;

    if (src == NULL) {
        memset(dst, 0, sizeof(*dst));
        return;
    }

    dst->exists = true;
    dst->pkind = src->pkind;
    dst->ckind = src->ckind;
    dst->color = src->costume_id;
    dst->stocks = src->stocks;
    dst->team = src->team;
    dst->damage = src->staminas.byName.damage_percent;

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Fighter* fp = Qol_GetUserDataOrNull(src->player_entity[i]);
        Bisim_CaptureFighter(&dst->fighters[i], fp);
        if (fp != NULL) {
            FRAY_ASSERT(fp->is_sub_fighter == i);
        }
    }
}

void Bisim_CaptureGlobal(Bisim_GlobalSnapshot* dst)
{
    size_t i;

    dst->seed = *HSD_RandSeedPtr;
    dst->curr_frame = gm_GetFrameCount();

    for (i = 0; i < GM_MAX_PLAYERS; i++) {
        StaticPlayer* pp = Player_GetPtrForSlot(i);
        Bisim_CapturePlayer(&dst->players[i], pp);
        if (pp != NULL) {
            FRAY_ASSERT(pp->player_id == i);
        }
    }
}

void Bisim_WriteFighter(BsIO_Cursor* c, const Bisim_FighterSnapshot* v)
{
    bsIO_WriteBool(c, v->exists);
    bsIO_WriteU8(c, v->ftkind);
    bsIO_WriteS16(c, v->msid);
    bsIO_WriteF32(c, v->anim_frame);
    bsIO_WriteBool(c, v->airborne);
    bsIO_WriteS8(c, v->facing_dir);
    bsIO_WriteVec3(c, &v->pos);
    bsIO_WriteVec3(c, &v->accel);
    bsIO_WriteVec3(c, &v->self_vel);
    bsIO_WriteVec3(c, &v->kb_vel);
    bsIO_WriteU32(c, v->buttons);
    bsIO_WriteVec2(c, &v->lstick);
    bsIO_WriteVec2(c, &v->cstick);
}

void Bisim_WritePlayer(BsIO_Cursor* c, const Bisim_PlayerSnapshot* v)
{
    size_t i;

    bsIO_WriteBool(c, v->exists);
    bsIO_WriteU8(c, v->pkind);
    bsIO_WriteU8(c, v->ckind);
    bsIO_WriteU8(c, v->color);
    bsIO_WriteS8(c, v->stocks);
    bsIO_WriteU8(c, v->team);
    bsIO_WriteU16(c, v->damage);

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Bisim_WriteFighter(c, &v->fighters[i]);
    }
}

void Bisim_WriteGlobal(BsIO_Cursor* c, const Bisim_GlobalSnapshot* v)
{
    size_t i;

    bsIO_WriteU32(c, v->seed);
    bsIO_WriteU32(c, v->curr_frame);

    for (i = 0; i < GM_MAX_PLAYERS; i++) {
        Bisim_WritePlayer(c, &v->players[i]);
    }
}

void Bisim_ReadFighter(BsIO_Cursor* c, Bisim_FighterSnapshot* v)
{
    v->exists = bsIO_ReadBool(c);
    v->ftkind = bsIO_ReadU8(c);
    v->msid = bsIO_ReadS16(c);
    v->anim_frame = bsIO_ReadF32(c);
    v->airborne = bsIO_ReadBool(c);
    v->facing_dir = bsIO_ReadS8(c);
    bsIO_ReadVec3(c, &v->pos);
    bsIO_ReadVec3(c, &v->accel);
    bsIO_ReadVec3(c, &v->self_vel);
    bsIO_ReadVec3(c, &v->kb_vel);
    v->buttons = bsIO_ReadU32(c);
    bsIO_ReadVec2(c, &v->lstick);
    bsIO_ReadVec2(c, &v->cstick);
}

void Bisim_ReadPlayer(BsIO_Cursor* c, Bisim_PlayerSnapshot* v)
{
    size_t i;

    v->exists = bsIO_ReadBool(c);
    v->pkind = bsIO_ReadU8(c);
    v->ckind = bsIO_ReadU8(c);
    v->color = bsIO_ReadU8(c);
    v->stocks = bsIO_ReadS8(c);
    v->team = bsIO_ReadU8(c);
    v->damage = bsIO_ReadU16(c);

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Bisim_ReadFighter(c, &v->fighters[i]);
    }
}

void Bisim_ReadGlobal(BsIO_Cursor* c, Bisim_GlobalSnapshot* v)
{
    size_t i;

    v->seed = bsIO_ReadU32(c);
    v->curr_frame = bsIO_ReadU32(c);

    for (i = 0; i < GM_MAX_PLAYERS; i++) {
        Bisim_ReadPlayer(c, &v->players[i]);
    }
    REPORT_UINT(c->pos);
    REPORT_UINT(c->cap);
}
