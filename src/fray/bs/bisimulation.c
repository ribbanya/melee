#include "bisimulation.h"

#include <abort_exit.h> // IWYU pragma: keep

#include "fray/bs/bsio.h"
#include "fray/lb/lbqol.h"
#include "melee/ft/forward.h"
#include "melee/gm/forward.h"
#include "melee/gm/gmvs.h"
#include "melee/pl/player.h"
#include "sysdolphin/baselib/random.h"
#include <melee/ft/types.h>
#include <sysdolphin/baselib/gobj.h>
#include <sysdolphin/baselib/gobjproc.h>

void Bisim_CaptureFighter(Bisim_FighterSnapshot* dst, const Fighter* src)
{
    if (!src) {
        bsIO_MemZero(dst, sizeof(*dst));
        return;
    }

    dst->exists = true;
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

    if (!src) {
        bsIO_MemZero(dst, sizeof(*dst));
        return;
    }

    dst->exists = true;
    dst->pkind = src->pkind;
    dst->color = src->costume_id;
    dst->port = src->controller_index;

    for (i = 0; i < PL_MAX_SUB_FIGHTERS; i++) {
        Fighter* fp = Qol_GetUserDataOrNull(src->player_entity[i]);
        Bisim_CaptureFighter(&dst->fighters[i], fp);
        if (fp) {
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
        if (pp) {
            FRAY_ASSERT(pp->player_id == i);
        }
    }
}

void Bisim_WriteFighter(BsIO_Cursor* c, const Bisim_FighterSnapshot* v)
{
    bsIO_WriteBool(c, v->exists);
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

    bsIO_WriteBool(c, v->exists);
    bsIO_WriteU8(c, v->pkind);
    bsIO_WriteU8(c, v->color);
    bsIO_WriteU8(c, v->port);

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
    v->msid = bsIO_ReadS32(c);
    v->anim_frame = bsIO_ReadF32(c);
    v->airborne = bsIO_ReadBool(c);
    bsIO_ReadVec3(c, &v->pos);
    bsIO_ReadVec3(c, &v->accel);
    bsIO_ReadVec3(c, &v->vel);
    bsIO_ReadVec3(c, &v->kb_vel);
}

void Bisim_SPrintGlobal(char* s, const Bisim_GlobalSnapshot* v)
{
    size_t i;
    s += sprintf(s,
                 "seed: %08X"
                 "\ncurr_frame: %u",
                 v->seed, v->curr_frame);
    for (i = 0; i < GM_MAX_PLAYERS; i++) {
        size_t j;
        const Bisim_PlayerSnapshot* pv = &v->players[i];
        if (!pv->exists) {
            continue;
        }
        s += sprintf(s,
                     "\nplayer[%u]:"
                     "\n\tpkind: %u"
                     "\n\tcolor: %u"
                     "\n\tport: %u",
                     i, pv->pkind, pv->color, pv->port);
        for (j = 0; j < PL_MAX_SUB_FIGHTERS; j++) {
            const Bisim_FighterSnapshot* fv = &pv->fighters[j];
            if (!fv->exists) {
                continue;
            }
            s += sprintf(s,
                         "\nfighter[%u]:"
                         "\n\t\tmsid: %d"
                         "\n\t\tairborne: %d"
                         "\n\t\tfacing_dir: %+.0f",
                         j, fv->msid, fv->airborne, fv->facing_dir);
        }
    }
}
