#include "itheart.h"

#include "gm/gm_1601.h"
#include "it/inlines.h"
#include "it/it_266F.h"
#include "it/it_26B1.h"
#include "it/itCommonItems.h"
#include "it/item.h"
#include "it/types.h"

#include <baselib/gobj.h>
#include <baselib/jobj.h>

void it_80283DD4(HSD_GObj* gobj);
void it_80283C7C(HSD_GObj* gobj);

Item_GObj* it_80283AE4(Item_GObj* gobj, Vec3* pos, s32 arg2)
{
    SpawnItem spawn;
    Item_GObj* item_gobj;

    item_gobj = NULL;
    if (gobj != NULL) {
        spawn.kind = It_Kind_Heart;
        spawn.prev_pos = *pos;
        spawn.prev_pos.z = 0.0F;
        spawn.pos = spawn.prev_pos;
        spawn.facing_dir = -1;
        spawn.x3C_damage = 0;
        spawn.vel.x = spawn.vel.y = spawn.vel.z = 0.0F;
        spawn.x0_parent_gobj = NULL;
        spawn.x4_parent_gobj2 = spawn.x0_parent_gobj;
        spawn.x44_flag.bits.b0 = true;
        spawn.x40 = 0;
        item_gobj = Item_80268B18(&spawn);
    }
    if (item_gobj != NULL) {
        Item* ip = GET_ITEM(item_gobj);
        ip->xDD4_itemVar.HeartContainer.xDD8.bits.b0 = true;
        ip->xDD4_itemVar.HeartContainer.xDDC = arg2;
        Item_80267454(item_gobj);
    }
    return item_gobj;
}

void it_80283BD4(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HeartContainerVars* vars = ip->xC4_article_data->x4_specialAttributes;

    ip->xDD4_itemVar.capsule.x0 = vars->xDD8.flags;
}

void it_80283BEC(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    HeartContainerVars* vars = ip->xC4_article_data->x4_specialAttributes;

    ip->x40_vel.x = 0;
    ip->x40_vel.y = vars->xDE8_float;
    ip->x40_vel.z = 0;
    ip->xDD4_itemVar.HeartContainer.xDD4_heal = vars->xDD4_heal;
    ip->xDD4_itemVar.HeartContainer.xDD8.bits.b0 = 0;
    ip->xDD4_itemVar.HeartContainer.xDDC = 0;
    it_80283DD4(gobj);
}

void it_80283C48(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);

    if ((s8) ((s32) ((ip->xDD4_itemVar.HeartContainer.xDD8.flags << 0x18) &
                     0xC0000000) >>
              0x1F) != 0)
    {
        M2C_FIELD((&gm_80473A18 + ip->xDD4_itemVar.HeartContainer.xDDC), s8*,
                  0x90) = 0;
    }
}

void it_80283C7C(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);

    it_8026B390(gobj);
    ip->x40_vel.x = 0.0F;
    ip->x40_vel.y = 0.0F;
    ip->x40_vel.z = 0.0F;
    Item_80268E5C(gobj, 0, ITEM_ANIM_UPDATE);
}

bool it_80283CD4(Item_GObj* gobj)
{
    HSD_JObj* child = HSD_JObjGetChild(GET_JOBJ(gobj));
    HeartContainerVars* attrs =
        GET_ITEM(gobj)->xC4_article_data->x4_specialAttributes;
    HSD_JObjAddRotationY(child, attrs->xDEC);
    return false;
}

void it_80283DA4(Item_GObj* gobj) {}

bool it_80283DA8(Item_GObj* gobj)
{
    it_8026D62C(gobj, it_80283DD4);
    return false;
}

void it_80283DD4(HSD_GObj* gobj)
{
    Item_80268E5C(gobj, 1, ITEM_ANIM_UPDATE);
}

bool it_80283DFC(Item_GObj* gobj)
{
    HSD_JObj* child = HSD_JObjGetChild(GET_JOBJ(gobj));
    HeartContainerVars* attrs =
        GET_ITEM(gobj)->xC4_article_data->x4_specialAttributes;
    HSD_JObjAddRotationY(child, attrs->xDEC);
    return false;
}

void it_80283ECC(Item_GObj* gobj)
{
    Item* item = GET_ITEM(gobj);
    ItemAttr* attr = item->xCC_item_attr;

    it_80272860(gobj, attr->x10_fall_speed, attr->x14_fall_speed_max);
}

bool it_80283EFC(Item_GObj* gobj)
{
    it_8026E15C(gobj, it_80283C7C);
    return false;
}

void it_80283F28(Item_GObj* gobj)
{
    HSD_JObj* child = HSD_JObjGetChild(GET_JOBJ(gobj));

    HSD_JObjClearFlagsAll(child, JOBJ_HIDDEN);
    HSD_JObjSetRotationY(child, 0.0F);
    Item_80268E5C(gobj, 2, ITEM_ANIM_UPDATE);
}

bool it_80284020(Item_GObj* gobj)
{
    return false;
}

void it_80284028(Item_GObj* gobj) {}

void it_8028402C(Item_GObj* gobj)
{
    HSD_JObj* child = HSD_JObjGetChild(GET_JOBJ(gobj));

    it_8026B390(gobj);
    HSD_JObjClearFlagsAll(child, JOBJ_HIDDEN);
    HSD_JObjSetRotationY(child, 0.0F);
    Item_80268E5C(gobj, 3, 6);
}

void it_8028412C(Item_GObj* gobj)
{
    Item_80268E5C(gobj, 4, ITEM_ANIM_UPDATE);
}

bool it_80284154(Item_GObj* gobj)
{
    return false;
}

void it_8028415C(Item_GObj* gobj) {}

bool it_80284160(Item_GObj* gobj)
{
    it_8026E8C4(gobj, it_80283C7C, it_80283DD4);
    return false;
}

void it_80284194(Item_GObj* gobj, Item_GObj* ref_gobj)
{
    it_8026B894(gobj, ref_gobj);
}

ItemStateTable it_803F56C8[] = {
    { 0xFFFFFFFF, it_80283CD4, it_80283DA4, it_80283DA8 },
    { 0xFFFFFFFF, it_80283DFC, it_80283ECC, it_80283EFC },
    { 0xFFFFFFFF, it_80284020, it_80284028, NULL },
    { 0xFFFFFFFF, it_80283DFC, it_80283ECC, it_80283EFC },
    { 0xFFFFFFFF, it_80284154, it_8028415C, it_80284160 },
};
