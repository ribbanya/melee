#include "itsamuschargeshot.h"

#include <placeholder.h>
#include <platform.h>

#include "baselib/forward.h"

#include "baselib/gobj.h"
#include "baselib/mtx.h"
#include "ef/eflib.h"
#include "ef/efsync.h"
#include "ft/chara/ftKirby/ftKb_Init.h"
#include "ft/chara/ftSamus/ftSs_SpecialN.h"
#include "ft/ftlib.h"
#include "ftSamus/ftSs_SpecialLw_0.h"

#include "it/forward.h"

#include "it/inlines.h"
#include "it/it_266F.h"
#include "it/it_26B1.h"
#include "it/it_2725.h"
#include "it/itCharItems.h"
#include "it/item.h"
#include "lb/lbvector.h"

#include <math.h>
#include <trigf.h>

ItemStateTable it_803F7288[] = {
    {
        0,
        itSamuschargeshot_UnkMotion0_Anim,
        itSamuschargeshot_UnkMotion0_Phys,
        itSamuschargeshot_UnkMotion0_Coll,
    },
    {
        1,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        2,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        3,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        4,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        5,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        6,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        7,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
    {
        8,
        itSamuschargeshot_UnkMotion8_Anim,
        itSamuschargeshot_UnkMotion8_Phys,
        itSamuschargeshot_UnkMotion8_Coll,
    },
};

bool it_802B5518(Item_GObj* gobj, CollData* cd)
{
    Item* ip = GET_ITEM(gobj);
    bool ret = false;
    it_8026DA08(gobj);
    if (ip->x40_vel.y > 0.0f) {
        if (cd->env_flags & Collide_CeilingMask) {
            ret = true;
        }
    } else if (cd->env_flags & Collide_FloorMask) {
        ret = true;
    }
    if (ip->x40_vel.x > 0.0f) {
        if (cd->env_flags & Collide_LeftWallMask) {
            ret = true;
        }
    } else if (cd->env_flags & Collide_RightWallMask) {
        ret = true;
    }
    return ret;
}

Item_GObj* it_802B55C8(Fighter_GObj* parent_gobj, Vec3* pos, Fighter_Part part,
                       ItemKind kind, float facing_dir)
{
    SpawnItem si;
    Item_GObj* gobj;

    si.kind = kind;
    si.prev_pos = *pos;
    si.prev_pos.z = 0.0f;
    it_8026BB68(parent_gobj, &si.pos);
    si.facing_dir = facing_dir;
    si.x3C_damage = 0;
    si.vel.x = si.vel.y = si.vel.z = 0.0f;
    si.x0_parent_gobj = parent_gobj;
    si.x4_parent_gobj2 = si.x0_parent_gobj;
    si.x44_flag.b0 = true;
    si.x40 = 0;
    gobj = Item_80268B18(&si);
    if (gobj != NULL) {
        Item* ip = GET_ITEM(gobj);
        itSamusChargeShot_Attributes* attr =
            ip->xC4_article_data->x4_specialAttributes;
        ip->xDAC_itcmd_var0 = ip->xDB0_itcmd_var1 = ip->xDB4_itcmd_var2 =
            ip->xDB8_itcmd_var3 = 0;
        it_2725_SetLifetime(gobj, attr->lifetime);
        ip->xDD4_itemVar.samuschargeshot.xDE8 = 0;
        ip->xDD4_itemVar.samuschargeshot.xDE0 = 0.0f;
        ip->xDD4_itemVar.samuschargeshot.xDFC = 0.0f;
        ip->xDD4_itemVar.samuschargeshot.parent_gobj = parent_gobj;
        ip->xDD4_itemVar.samuschargeshot.xE04 = ftLib_800869D4(parent_gobj);
        ip->xDD4_itemVar.samuschargeshot.xE2C = 0.0f;
        ip->xDD4_itemVar.samuschargeshot.xE30 = 0.0f;
        ip->xDD4_itemVar.samuschargeshot.xE34 = 0.0f;
        Item_8026AB54(gobj, parent_gobj, part);
    }
    return gobj;
}

void it_802B56E4(Item_GObj* gobj, Vec3* vec, f32 farg0, f32 farg1, f32 farg2)
{
    Item* ip = GET_ITEM(gobj);
    itSamusChargeShot_Attributes* attr =
        ip->xC4_article_data->x4_specialAttributes;
    PAD_STACK(8);

    ip->xDD4_itemVar.samuschargeshot.xDD8 = farg0;
    it_2725_SetLifetime(gobj, attr->lifetime);
    if (farg1 < 0.0f) {
        farg1 = 0.0f;
    }
    if (farg1 > farg2) {
        farg1 = farg2;
    }
    ip->xDD4_itemVar.samuschargeshot.xDEC = farg1;
    ip->xDD4_itemVar.samuschargeshot.xDF0 = farg2;
    {
        if (ip->xDD4_itemVar.samuschargeshot.parent_gobj != NULL &&
            ip->owner == ip->xDD4_itemVar.samuschargeshot.parent_gobj)
        {
            it_802B5CBC(gobj, ip->xDD4_itemVar.samuschargeshot.parent_gobj);
            {
                Vec3 sp2C;
                Vec3 sp20;

                HSD_MtxGetRotation(
                    ftLib_80086630(
                        ip->xDD4_itemVar.samuschargeshot.parent_gobj, ip->xDC4)
                        ->mtx,
                    &sp2C);
                sp20.x = sp20.y = sp20.z = 0.0f;
                it_8027429C(gobj, &sp20);
            }
            ip->xDC8_word.flags.x14 = 0;
            it_8026B3A8(gobj);
            ip->xDD4_itemVar.samuschargeshot.xDDC =
                (farg1 * ((attr->xC - attr->x8) / farg2)) + attr->x8;
            ip->xDD4_itemVar.samuschargeshot.xDF8 =
                (u32) ((farg1 * ((attr->x14 - attr->x10) / farg2)) +
                       attr->x10);
            ip->xDD4_itemVar.samuschargeshot.xDE0 = 0.0f;
            ip->xDD4_itemVar.samuschargeshot.xDE4 =
                (ip->xDD4_itemVar.samuschargeshot.xDEC *
                 ((attr->x1C - attr->x18) /
                  ip->xDD4_itemVar.samuschargeshot.xDF0)) +
                attr->x18;
            ip->xDD4_itemVar.samuschargeshot.xDF4 = 0;
            ip->facing_dir = ftLib_GetFacingDir(
                ip->xDD4_itemVar.samuschargeshot.parent_gobj);
            ip->pos = *vec;
            ip->xDD4_itemVar.samuschargeshot.xDFC = 0;
            ip->x40_vel.x = ip->xDD4_itemVar.samuschargeshot.xDDC *
                            cosf(ip->xDD4_itemVar.samuschargeshot.xDD8);
            ip->x40_vel.y = ip->xDD4_itemVar.samuschargeshot.xDDC *
                            sinf(ip->xDD4_itemVar.samuschargeshot.xDD8);
            ip->x40_vel.z = 0.0f;
            ip->xDD4_itemVar.samuschargeshot.xDE8 = 1;
        }
    }
}

void it_2725_Logic108_Destroyed(Item_GObj* gobj)
{
    if (gobj != NULL) {
        Item* ip = GET_ITEM(gobj);
        efLib_DestroyAll((HSD_GObj*) gobj);
        if (ip->xDD4_itemVar.samuschargeshot.xDE8 == 0) {
            if (ip->xDD4_itemVar.samuschargeshot.parent_gobj != NULL &&
                ip->owner == ip->xDD4_itemVar.samuschargeshot.parent_gobj)
            {
                switch (ip->kind) {
                case It_Kind_Samus_Charge:
                    ftSs_SpecialN_801291F0(
                        ip->xDD4_itemVar.samuschargeshot.parent_gobj);
                    break;
                case It_Kind_Kirby_SamusCharge:
                    ftKb_SpecialNSs_800FCD04(
                        ip->xDD4_itemVar.samuschargeshot.parent_gobj);
                    break;
                default:
                    break;
                }
            }
            ip->xDD4_itemVar.samuschargeshot.parent_gobj = NULL;
        }
        ip->owner = NULL;
        ip->xDC8_word.flags.x13 = 0;
    }
}

void it_802B5974(Item_GObj* gobj)
{
    if (gobj != NULL) {
        Item* ip = GET_ITEM(gobj);
        efLib_DestroyAll((HSD_GObj*) gobj);
        ip->xDD4_itemVar.samuschargeshot.xDFC = 0;
        Item_8026A8EC(gobj);
    }
}

bool itSamuschargeshot_UnkMotion0_Anim(Item_GObj* gobj)
{
    Point3d scale;
    HSD_JObj* jobj;
    HSD_JObj* grandchild_jobj;
    HSD_JObj* var_r3;
    f32 temp_f0;

    Item* ip = GET_ITEM(gobj);
    itSamusChargeShot_Attributes* sa =
        ip->xC4_article_data->x4_specialAttributes;
    grandchild_jobj = itGetJObjGrandchild(gobj);
    if ((ip->xDD4_itemVar.samuschargeshot.parent_gobj != NULL) &&
        (ip->owner == ip->xDD4_itemVar.samuschargeshot.parent_gobj))
    {
        switch (ip->kind) {
        case It_Kind_Samus_Charge:
            if (ftSs_SpecialLw_80129158(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj) != 0)
            {
                return true;
            }
            if (ftSs_SpecialN_801291A8(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj) != 0)
            {
                return true;
            }
            if (ftSs_SpecialLw_80129100(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj,
                    &ip->xDD4_itemVar.samuschargeshot.xDEC,
                    &ip->xDD4_itemVar.samuschargeshot.xDF0) == -1)
            {
                return true;
            }
            goto block_24;
        case It_Kind_Kirby_SamusCharge:
            if (ftKb_SpecialNSs_800FCC6C(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj) != 0)
            {
                return true;
            }
            if (ftKb_SpecialNSs_800FCCBC(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj) != 0)
            {
                return true;
            }
            if (ftKb_SpecialNSs_800FCC14(
                    ip->xDD4_itemVar.samuschargeshot.parent_gobj,
                    &ip->xDD4_itemVar.samuschargeshot.xDEC,
                    &ip->xDD4_itemVar.samuschargeshot.xDF0) == -1)
            {
                return true;
            }
            goto block_24;
        default:
            break;
        }
    } else {
    block_24: {
        float temp_f3 = sa->x18;
        temp_f0 =
            (ip->xDD4_itemVar.samuschargeshot.xDEC *
             ((sa->x1C - temp_f3) / ip->xDD4_itemVar.samuschargeshot.xDF0)) +
            temp_f3;
        scale.x = scale.y = scale.z = temp_f0;
        HSD_JObjSetScale(grandchild_jobj, &scale);
        return false;
    }
    }
}

void it_2725_Logic108_PickedUp(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HSD_JObj* jobj = itGetJObjGrandchild(gobj);

    Item_80268E5C(gobj, 0, ITEM_ANIM_UPDATE);
    switch (ip->kind) {
    case It_Kind_Samus_Charge:
        efSync_Spawn(0x47F, gobj, jobj);
        ip->xDD4_itemVar.samuschargeshot.xDFC = 1;
        break;
    case It_Kind_Kirby_SamusCharge:
        efSync_Spawn(0x4A1, gobj, jobj);
        ip->xDD4_itemVar.samuschargeshot.xDFC = 1;
        break;
    default:
        break;
    }
}

bool itSamuschargeshot_UnkMotion8_Anim(Item_GObj* gobj)
{
    Item* ip = gobj->user_data;
    HSD_JObj* jobj = itGetJObjGrandchild(gobj);
    PAD_STACK(4);
    {
        Vec3 scale;

        scale.x = scale.y = scale.z = ip->xDD4_itemVar.samuschargeshot.xDE4;

        HSD_JObjSetScale(jobj, &scale);
        return it_80273130(gobj);
    }
}

void itSamuschargeshot_UnkMotion0_Phys(Item_GObj* gobj) {}

bool itSamuschargeshot_UnkMotion0_Coll(Item_GObj* gobj)
{
    return false;
}

void it_802B5CBC(Item_GObj* gobj, Fighter_GObj* unused)
{
    Item* ip = GET_ITEM(gobj);
    itSamusChargeShot_Attributes* attr =
        ip->xC4_article_data->x4_specialAttributes;

    it_2725_SetLifetime(gobj, attr->lifetime);
    efLib_DestroyAll(gobj);
    ip->xDD4_itemVar.samuschargeshot.xDFC = 0.0f;

    if (ip->xDD4_itemVar.samuschargeshot.xDEC < 0) {
        ip->xDD4_itemVar.samuschargeshot.xDEC = 0;
    }
    if (ip->xDD4_itemVar.samuschargeshot.xDEC >= 8) {
        ip->xDD4_itemVar.samuschargeshot.xDEC = 7;
    }
    Item_80268E5C(gobj, ip->xDD4_itemVar.samuschargeshot.xDEC + 1,
                  ITEM_ANIM_UPDATE);
    ip->on_accessory = it_802B5EDC;
}

void itSamuschargeshot_UnkMotion8_Phys(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    ip->x40_vel.x = ip->xDD4_itemVar.samuschargeshot.xDDC *
                    cosf(ip->xDD4_itemVar.samuschargeshot.xDD8);
    ip->x40_vel.y = ip->xDD4_itemVar.samuschargeshot.xDDC *
                    sinf(ip->xDD4_itemVar.samuschargeshot.xDD8);
}

bool itSamuschargeshot_UnkMotion8_Coll(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    if (it_802B5518(gobj, &ip->x378_itemColl)) {
        return true;
    }
    return false;
}

void it_802B5EDC(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HSD_JObj* jobj = GET_JOBJ(gobj);

    if ((ip->xDD4_itemVar.samuschargeshot.xDEC ==
         ip->xDD4_itemVar.samuschargeshot.xDF0) &&
        (ip->xDD4_itemVar.samuschargeshot.xDF4 == 0))
    {
        switch (ip->kind) {
        case It_Kind_Samus_Charge:
            efSync_Spawn(0x480, gobj, jobj);
            efSync_Spawn(0x481, gobj, jobj);
            break;
        case It_Kind_Kirby_SamusCharge:
            efSync_Spawn(0x4A2, gobj, jobj);
            efSync_Spawn(0x4A3, gobj, jobj);
            break;
        default:
            break;
        }
    }
    ip->xDD4_itemVar.samuschargeshot.xDF4 =
        ip->xDD4_itemVar.samuschargeshot.xDF4 + 1;
    ip->xDD4_itemVar.samuschargeshot.xDF4 =
        ip->xDD4_itemVar.samuschargeshot.xDF4 % 3;
}

bool itSamusChargeshot_Logic108_DmgDealt(Item_GObj* arg0)
{
    return true;
}

bool itSamusChargeshot_Logic108_Clanked(Item_GObj* arg0)
{
    return true;
}

bool itSamusChargeshot_Logic108_Absorbed(Item_GObj* arg0)
{
    return true;
}

bool it_2725_Logic108_Reflected(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HSD_JObj* jobj = GET_JOBJ(gobj);
    f32 pi_over_two_x_facing_dir;

    ip->facing_dir = -ip->facing_dir;
    pi_over_two_x_facing_dir = (f32) (M_PI_2 * (f64) ip->facing_dir);
    HSD_JObjSetRotationY(jobj, pi_over_two_x_facing_dir);

    ip->xDD4_itemVar.samuschargeshot.xDD8 += M_PI;
    while (ip->xDD4_itemVar.samuschargeshot.xDD8 < 0.0f) {
        ip->xDD4_itemVar.samuschargeshot.xDD8 += M_TAU;
    }
    while (ip->xDD4_itemVar.samuschargeshot.xDD8 > M_TAU) {
        ip->xDD4_itemVar.samuschargeshot.xDD8 -= M_TAU;
    }
    return false;
}

bool itSamusChargeshot_Logic108_HitShield(Item_GObj* arg0)
{
    return true;
}

bool it_2725_Logic108_ShieldBounced(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HSD_JObj* jobj = GET_JOBJ(gobj);
    f32 pi_over_two_x_facing_dir;

    lbVector_Mirror(&ip->x40_vel, &ip->xC58);
    ip->xDD4_itemVar.samuschargeshot.xDD8 =
        atan2f(ip->x40_vel.y, ip->x40_vel.x);
    while (ip->xDD4_itemVar.samuschargeshot.xDD8 < 0.0f) {
        ip->xDD4_itemVar.samuschargeshot.xDD8 += M_TAU;
    }
    while (ip->xDD4_itemVar.samuschargeshot.xDD8 > M_TAU) {
        ip->xDD4_itemVar.samuschargeshot.xDD8 -= M_TAU;
    }

    ip->facing_dir = (ip->x40_vel.x >= 0.0f) ? 1.0f : -1.0f;
    pi_over_two_x_facing_dir = (f32) (M_PI_2 * (f64) ip->facing_dir);
    HSD_JObjSetRotationY(jobj, pi_over_two_x_facing_dir);
    return false;
}

void itSamusChargeshot_Logic108_EvtUnk(Item_GObj* gobj, Item_GObj* ref_gobj)
{
    it_8026B894(gobj, ref_gobj);
}
