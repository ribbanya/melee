#include "it/forward.h"

#include "itlgunbeam.h"

#include "db/db_2253.h"
#include "ef/eflib.h"
#include "ef/efsync.h"
#include "it/inlines.h"
#include "it/it_266F.h"
#include "it/it_26B1.h"
#include "it/types.h"
#include "lb/lbvector.h"

#include <math.h>
#include <baselib/gobj.h>
#include <baselib/random.h>
#include <melee/it/item.h>
#include <melee/lb/lbrefract.h>

UNK_RET it_80272940(Item_GObj*);              /* extern */
void it_802998A0(Item_GObj*, HSD_GObj*, s32); /* static */
int it_8029999C(HSD_GObj*);                   /* static */
void it_802999E4(HSD_GObj*);                  /* static */
int it_80299A68(HSD_GObj*);                   /* static */

static f32 it_804DCC20 = 0.0f;
static double it_804DCC28 = 2 * M_PI;
static double it_804DCC30 = -M_PI;
static double it_804DCC38 = M_PI;
static double it_804DCC40 = M_PI_2;
static double it_804DCC48 = 0.02;
static double it_804DCC50 = 0.5;
static f32 it_804DCC58 = 1.0f;
static f32 it_804DCC5C = 3.0f;

ItemStateTable it_803F6630[1] = {
    { 0, it_8029999C, it_802999E4, it_80299A68 },
};

void it_802993E0(Item_GObj* item_gobj, s32 arg1)
{
    // f32 sp1C;
    // f32 sp18;
    // f32 sp14;
    Vec3 pos;
    Item* item;

    item = item_gobj->user_data;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    // sp14 = 0.0f;
    // sp18 = 0.0f;
    // sp1C = 0.0f;
loop_2:
    if (item->xDD4_itemVar.lgunbeam.angle0 < -M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 + 2 * M_PI;
        goto loop_2;
    }
loop_5:
    if (item->xDD4_itemVar.lgunbeam.angle0 > M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 - 2 * M_PI;
        goto loop_5;
    }
#if 0
    switch (arg1) {
    case 1:
        pos.x += item->x378_itemColl.floor.normal.x;
        pos.y += item->x378_itemColl.floor.normal.y;
    case 2:
        pos.x += item->x378_itemColl.ceiling.normal.x;
        pos.y += item->x378_itemColl.ceiling.normal.y;
    case 4:
        pos.x += item->x378_itemColl.right_wall.normal.x;
        pos.y += item->x378_itemColl.right_wall.normal.y;
    case 8:
        pos.x += item->x378_itemColl.left_wall.normal.x;
        pos.y += item->x378_itemColl.left_wall.normal.y;
    }
    lbVector_Normalize(&pos);
    item->xDD4_itemVar.lgunbeam.xDE0.x = pos.x;
    item->xDD4_itemVar.lgunbeam.xDE0.y = pos.y;
    item->xDD4_itemVar.lgunbeam.xDE0.z = pos.z;
#elif 1
    if (arg1 & 1) {
        pos.x += item->x378_itemColl.floor.normal.x;
        pos.y += item->x378_itemColl.floor.normal.y;
        // sp14 += item->x378_itemColl.floor.normal.x;
        // sp18 += item->x378_itemColl.floor.normal.y;
    }
    if (arg1 & 2) {
        pos.x += item->x378_itemColl.ceiling.normal.x;
        pos.y += item->x378_itemColl.ceiling.normal.y;
        // sp14 += item->x378_itemColl.ceiling.normal.x;
        // sp18 += item->x378_itemColl.ceiling.normal.y;
    }
    if (arg1 & 4) {
        pos.x += item->x378_itemColl.right_wall.normal.x;
        pos.y += item->x378_itemColl.right_wall.normal.y;
        // sp14 += item->x378_itemColl.right_wall.normal.x;
        // sp18 += item->x378_itemColl.right_wall.normal.y;
    }
    if (arg1 & 8) {
        pos.x += item->x378_itemColl.left_wall.normal.x;
        pos.y += item->x378_itemColl.left_wall.normal.y;
        // sp14 += item->x378_itemColl.left_wall.normal.x;
        // sp18 += item->x378_itemColl.left_wall.normal.y;
    }
    lbVector_Normalize(&pos);
    item->xDD4_itemVar.lgunbeam.position1.x = pos.x;
    item->xDD4_itemVar.lgunbeam.position1.y = pos.y;
    item->xDD4_itemVar.lgunbeam.position1.z = pos.z;
#else
    if (arg1 & 1) {
        sp14 += item->x378_itemColl.floor.normal.x;
        sp18 += item->x378_itemColl.floor.normal.y;
    }
    if (arg1 & 2) {
        sp14 += item->x378_itemColl.ceiling.normal.x;
        sp18 += item->x378_itemColl.ceiling.normal.y;
    }
    if (arg1 & 4) {
        sp14 += item->x378_itemColl.right_wall.normal.x;
        sp18 += item->x378_itemColl.right_wall.normal.y;
    }
    if (arg1 & 8) {
        sp14 += item->x378_itemColl.left_wall.normal.x;
        sp18 += item->x378_itemColl.left_wall.normal.y;
    }
    lbVector_Normalize((Vec3*) &sp14);
    item->xDD4_itemVar.lgunbeam.xDE0.x = sp14;
    item->xDD4_itemVar.lgunbeam.xDE0.y = sp18;
    item->xDD4_itemVar.lgunbeam.xDE0.z = sp1C;
#endif
}

void it_80299528(Item_GObj* gobj, s32 arg1)
{
    Item* ip = GET_ITEM(gobj);

    if (arg1 != 0) {
        float angle1 = atan2f(ip->xDD4_itemVar.lgunbeam.position1.x,
                              ip->xDD4_itemVar.lgunbeam.position1.y);
        float angle2 = atan2f(ip->xDD4_itemVar.lgunbeam.position2.x,
                              ip->xDD4_itemVar.lgunbeam.position2.y) -
                       angle1;
        while (angle2 > M_PI) {
            angle2 -= 2 * M_PI;
        }
        while (angle2 < -M_PI) {
            angle2 += 2 * M_PI;
        }
        {
            float var_f1;
            if (angle2 == 0.0f) {
                var_f1 = 0.0f;
            } else {
                float temp_f1 =
                    lbVector_Angle(&ip->xDD4_itemVar.lgunbeam.position1,
                                   &ip->xDD4_itemVar.lgunbeam.position2);
                {
                    float var_f2_2;
                    if (ABS(angle2) < M_PI_2) {
                        var_f2_2 = 0.02f * (ABS(angle2) / M_PI);
                    } else {
                        var_f2_2 = 0.5 * (ABS(angle2) / M_PI);
                    }
                    {
                        float var_f0;
                        if (angle2 < 0.0f) {
                            var_f0 = -var_f2_2;
                        } else {
                            var_f0 = var_f2_2;
                        }
                        var_f1 = temp_f1 * var_f0;
                    }
                }
            }
            ip->xDD4_itemVar.lgunbeam.angle0 -= var_f1;
        }
        while (ip->xDD4_itemVar.lgunbeam.angle0 < -M_PI) {
            ip->xDD4_itemVar.lgunbeam.angle0 += 2 * M_PI;
        }
        while (ip->xDD4_itemVar.lgunbeam.angle0 > +M_PI) {
            ip->xDD4_itemVar.lgunbeam.angle0 -= 2 * M_PI;
        }
    }
}

void it_802996D0(HSD_GObj* owner_gobj, Vec3* pos, u32 arg2, f32 facing_dir)
{
    SpawnItem spawn;
    PAD_STACK(4);

    spawn.kind = It_Kind_L_Gun_Beam;
    spawn.prev_pos = *pos;
    spawn.prev_pos.z = 0.0f;
    it_8026BB68(owner_gobj, &spawn.pos);
    spawn.facing_dir = facing_dir;
    spawn.x3C_damage = 0;
    spawn.vel.x = spawn.vel.y = spawn.vel.z = 0.0f;
    spawn.x0_parent_gobj = owner_gobj;
    spawn.x4_parent_gobj2 = spawn.x0_parent_gobj;
    spawn.x44_flag.bits.b0 = true;
    spawn.x40 = arg2;

    {
        Item_GObj* gobj = Item_80268B18(&spawn);
        if (gobj != NULL) {
            Item* item = GET_ITEM(gobj);
            ItLGunBeamAttr* item_spec_attr =
                item->xC4_article_data->x4_specialAttributes;
            item->xDAC_itcmd_var0 = item->xDB0_itcmd_var1 =
                item->xDB4_itcmd_var2 = item->xDB8_itcmd_var3 = 0;
            item->xDD4_itemVar.lgunbeam.lifetime = item_spec_attr->x0;
            it_80275158(gobj, item_spec_attr->x0);
            item->xDD4_itemVar.lgunbeam.position0 = *pos;
            item->xDD4_itemVar.lgunbeam.angle1 =
                ((item_spec_attr->x8 - item_spec_attr->x4) * HSD_Randf()) +
                item_spec_attr->x4;
            item->xDD4_itemVar.lgunbeam.angle0 =
                (item_spec_attr->x10 - item_spec_attr->xC) * HSD_Randf() +
                item_spec_attr->xC;
            {
                f32 angle;
                if (item->facing_dir == +1) {
                    angle = +item->xDD4_itemVar.lgunbeam.angle0;
                } else {
                    angle = -item->xDD4_itemVar.lgunbeam.angle0;
                }
                item->xDD4_itemVar.lgunbeam.angle0 = angle;
            }
            while (item->xDD4_itemVar.lgunbeam.angle0 < -M_PI) {
                item->xDD4_itemVar.lgunbeam.angle0 += 2 * M_PI;
            }
            while (item->xDD4_itemVar.lgunbeam.angle0 > M_PI) {
                item->xDD4_itemVar.lgunbeam.angle0 -= 2 * M_PI;
            }
            item->xDD4_itemVar.lgunbeam.xE04 = 0.0f;
            item->xDCC_flag.b3 = false;

            it_802998A0(gobj, owner_gobj, HSD_Randi(4));
        }
    }
}

void it_802998A0(Item_GObj* item_gobj, HSD_GObj* fighter_gobj, s32 arg2)
{
    f32 sp2C;
    Vec3 pos;
    HSD_JObj* item_jobj;
    Item* item;
    f32 temp_f1;
    f32 unused1;
    f32 unused2;
    f32 unused3;

    item = item_gobj->user_data;
    item_jobj = item_gobj->hsd_obj;
    sp2C = 1.0f;
    it_8026B3A8(item_gobj);
    // item->unkDCA = item->unkDCA & ~0x10;
    item->xDC8_word.flags.x3 = 0;
    it_80272940(item_gobj);
    Item_80268E5C((HSD_GObj*) item_gobj, 0, ITEM_ANIM_UPDATE);
    pos = item->pos;
    Item_802694CC((HSD_GObj*) item_gobj);
    it_802999E4((HSD_GObj*) item_gobj);
    item->pos.x -= item->x40_vel.x;
    item->pos.y -= item->x40_vel.y;
    temp_f1 = it_8026D9A0(item_gobj);
    // item->pos = pos;
    // item->pos.x = sp30;
    // item->pos.y = temp_r0;
    // item->pos.z = temp_r0_2;
    // efSync_Spawn(arg2 + 0x44E, item_gobj, item_jobj, item->0x2C, &sp2C,
    // temp_f1); efSync_Spawn(arg2 + 0x44E, item_gobj, item_jobj,
    // item->facing_dir, pos, &sp2C);
    efSync_Spawn(arg2 + 0x44E, item_gobj, item_jobj, item->facing_dir,
                 item->pos, &sp2C);
    db_80225DD8(item_gobj, (Fighter_GObj*) fighter_gobj);
}

int it_8029999C(HSD_GObj* item_gobj)
{
    f32 unused1;
    f32 unused2;
    f32 unused3;
    f32 unused4;

    if (it_80273130((Item_GObj*) item_gobj) == 1) {
        efLib_DestroyAll((HSD_GObj*) item_gobj);
        return 1;
    }
    return 0;
}

void it_802999E4(HSD_GObj* item_gobj)
{
    Item* item;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 unused1;
    f32 unused2;

    item = item_gobj->user_data;
    item->xDD4_itemVar.lgunbeam.position0 = item->pos;
    temp_f0 = item->xDD4_itemVar.lgunbeam.angle1 *
              sinf(item->xDD4_itemVar.lgunbeam.angle0);
    item->x40_vel.x = temp_f0;
    // item->xDD4_itemVar.lgunbeam.xDEC = temp_f0;
    item->xDD4_itemVar.lgunbeam.position2.x = temp_f0;
    temp_f0_2 = item->xDD4_itemVar.lgunbeam.angle1 *
                cosf(item->xDD4_itemVar.lgunbeam.angle0);
    item->x40_vel.y = temp_f0_2;
    item->xDD4_itemVar.lgunbeam.position2.y = temp_f0_2;
    item->x40_vel.z = 0.0f;
    item->xDD4_itemVar.lgunbeam.position2.z = 0.0f;
    lbVector_Normalize(&item->xDD4_itemVar.lgunbeam.position2);
}

int it_80299A68(HSD_GObj* item_gobj)
{
    Item* item;
    s32 temp_r3;
    s32 var_r31;

    item = item_gobj->user_data;
loop_2:
    if (item->xDD4_itemVar.lgunbeam.angle0 < -M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 + 2 * M_PI;
        goto loop_2;
    }
loop_5:
    if (item->xDD4_itemVar.lgunbeam.angle0 > M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 - 2 * M_PI;
        goto loop_5;
    }
    var_r31 = 0;
    // item->x378_itemColl.x108_joint = 3.0f;
    // item->x378_itemColl.x10C_joint[0] = 3.0f;
    // item->x378_itemColl.x10C_joint[1] = 3.0f;
    // item->x378_itemColl.x10C_joint[2] = 3.0f;
    item->x378_itemColl.x108_f32 = 3.0f;
    item->x378_itemColl.x10C_f32 = 3.0f;
    item->x378_itemColl.x110_f32 = 3.0f;
    item->x378_itemColl.x114_f32 = 3.0f;
    it_8026D9A0((Item_GObj*) item_gobj);
    temp_r3 = item->x378_itemColl.env_flags;
    if (temp_r3 & 0x18000) {
        var_r31 = 1;
    }
    if (temp_r3 & 0x6000) {
        var_r31 |= 2;
    }
    if (temp_r3 & 0x3F) {
        var_r31 |= 4;
    }
    if (temp_r3 & 0xFC0) {
        var_r31 |= 8;
    }
    if (var_r31 != 0) {
        it_802993E0((Item_GObj*) item_gobj, var_r31);
        it_80299528((Item_GObj*) item_gobj, var_r31);
    }
    return 0;
}

int it_80299B6C(Item_GObj* item_gobj)
{
    return 0;
}

int it_80299B74(Item_GObj* item_gobj)
{
    Item* item;

    item = item_gobj->user_data;
    item->xDD4_itemVar.lgunbeam.angle0 =
        item->xDD4_itemVar.lgunbeam.angle0 + M_PI;
loop_2:
    if (item->xDD4_itemVar.lgunbeam.angle0 < -M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 + 2 * M_PI;
        goto loop_2;
    }
loop_5:
    if (item->xDD4_itemVar.lgunbeam.angle0 > M_PI) {
        item->xDD4_itemVar.lgunbeam.angle0 =
            item->xDD4_itemVar.lgunbeam.angle0 - 2 * M_PI;
        goto loop_5;
    }
    item->facing_dir = -item->facing_dir;
    item->x40_vel.x = -item->x40_vel.x;
    item->x40_vel.y = -item->x40_vel.y;
    return 0;
}

int it_80299C08(Item_GObj* item_gobj)
{
    return 1;
}

int it_80299C10(Item_GObj* item_gobj)
{
    return 0;
}

int it_80299C18(Item_GObj* item_gobj)
{
    return 1;
}

int it_80299C20(Item_GObj* item_gobj)
{
    return 1;
}

void it_80299C28(Item_GObj* item_gobj, HSD_GObj* ref_gobj)
{
    it_8026B894(item_gobj, ref_gobj);
}
