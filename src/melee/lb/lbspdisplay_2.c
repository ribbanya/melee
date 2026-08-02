#include "lbspdisplay.static.h"

#include "stddef.h"

#include "baselib/displayfunc.h"
#include "baselib/psstructs.h"
#include "baselib/rumble.h"
#include "baselib/tobj.h"
#include "dolphin/gx/GXCull.h"
#include "dolphin/gx/GXGeometry.h"
#include "dolphin/gx/GXLighting.h"
#include "dolphin/gx/GXPixel.h"
#include "dolphin/gx/GXTev.h"
#include "dolphin/gx/GXTransform.h"
#include "dolphin/gx/GXVert.h"
#include "dolphin/pad.h"

#include "lb/forward.h"

#include "lb/lbarchive.h"
#include "lb/lbcommand.h"
#include "lb/types.h"

#include <math_ppc.h>
#include <trigf.h>
#include <baselib/cobj.h>
#include <baselib/dobj.h>
#include <baselib/gobj.h>
#include <baselib/gobjobject.h>
#include <baselib/jobj.h>
#include <baselib/lobj.h>

/* 4D63C0 */ static struct Fighter_804D653C_t* lb_804D63C0;

typedef bool (*lb_803BA248_fn)(ColorOverlay*);

bool lb_80013C18(ColorOverlay* arg0)
{
    arg0->x7C_light_enable = arg0->x8_ptr1->light_rot2.light_enable;
    arg0->x74_light_rot_x = arg0->x8_ptr1->light_rot2.x;
    arg0->x78_light_rot_yz = arg0->x8_ptr1->light_rot2.yz;
    ++arg0->x8_ptr1;
    arg0->x50_light_color.r = arg0->x8_ptr1->light_color.r;
    arg0->x50_light_color.g = arg0->x8_ptr1->light_color.g;
    arg0->x50_light_color.b = arg0->x8_ptr1->light_color.b;
    arg0->x50_light_color.a = arg0->x8_ptr1->light_color.a;
    arg0->x54_light_red = arg0->x50_light_color.r;
    arg0->x58_light_green = arg0->x50_light_color.g;
    arg0->x5C_light_blue = arg0->x50_light_color.b;
    arg0->x60_light_alpha = arg0->x50_light_color.a;
    arg0->x70_lightblend_alpha = 0.0f;
    arg0->x6C_lightblend_blue = 0.0f;
    arg0->x68_lightblend_green = 0.0f;
    arg0->x64_lightblend_red = 0.0f;
    ++arg0->x8_ptr1;
    arg0->x7C_flag2 = true;
    return false;
}

bool lb_80013D68(ColorOverlay* arg0)
{
    ++arg0->x8_ptr1;
    arg0->x50_light_color.r = arg0->x8_ptr1->light_color.r;
    arg0->x50_light_color.g = arg0->x8_ptr1->light_color.g;
    arg0->x50_light_color.b = arg0->x8_ptr1->light_color.b;
    arg0->x50_light_color.a = arg0->x8_ptr1->light_color.a;
    arg0->x54_light_red = arg0->x50_light_color.r;
    arg0->x58_light_green = arg0->x50_light_color.g;
    arg0->x5C_light_blue = arg0->x50_light_color.b;
    arg0->x60_light_alpha = arg0->x50_light_color.a;
    arg0->x70_lightblend_alpha = 0.0f;
    arg0->x6C_lightblend_blue = 0.0f;
    arg0->x68_lightblend_green = 0.0f;
    arg0->x64_lightblend_red = 0.0f;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_80013E3C(ColorOverlay* arg0)
{
    float f = arg0->x8_ptr1++->unk.timer;
    arg0->x64_lightblend_red =
        ((0.5f + arg0->x8_ptr1->light_color.r) - arg0->x50_light_color.r) / f;
    arg0->x68_lightblend_green =
        ((0.5f + arg0->x8_ptr1->light_color.g) - arg0->x50_light_color.g) / f;
    arg0->x6C_lightblend_blue =
        ((0.5f + arg0->x8_ptr1->light_color.b) - arg0->x50_light_color.b) / f;
    arg0->x70_lightblend_alpha =
        ((0.5f + arg0->x8_ptr1->light_color.a) - arg0->x50_light_color.a) / f;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_80013F78(ColorOverlay* arg0)
{
    arg0->x74_light_rot_x = arg0->x8_ptr1->light_rot1.x;
    arg0->x78_light_rot_yz = arg0->x8_ptr1->light_rot1.yz;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_80013FF0(ColorOverlay* arg0)
{
    arg0->x7C_flag2 = false;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_80014014(ColorOverlay* arg0)
{
    arg0->x7C_color_enable = true;
    ++arg0->x8_ptr1;
    arg0->x2C_hex.r = arg0->x8_ptr1->light_color.r;
    arg0->x2C_hex.g = arg0->x8_ptr1->light_color.g;
    arg0->x2C_hex.b = arg0->x8_ptr1->light_color.b;
    arg0->x2C_hex.a = arg0->x8_ptr1->light_color.a;
    arg0->x30_color_red = arg0->x2C_hex.r;
    arg0->x34_color_green = arg0->x2C_hex.g;
    arg0->x38_color_blue = arg0->x2C_hex.b;
    arg0->x3C_color_alpha = arg0->x2C_hex.a;
    arg0->x4C_colorblend_alpha = 0.0f;
    arg0->x48_colorblend_blue = 0.0f;
    arg0->x44_colorblend_green = 0.0f;
    arg0->x40_colorblend_red = 0.0f;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_800140F8(ColorOverlay* arg0)
{
    float f = arg0->x8_ptr1++->unk.timer;
    arg0->x40_colorblend_red =
        ((0.5f + arg0->x8_ptr1->light_color.r) - arg0->x2C_hex.r) / f;
    arg0->x44_colorblend_green =
        ((0.5f + arg0->x8_ptr1->light_color.g) - arg0->x2C_hex.g) / f;
    arg0->x48_colorblend_blue =
        ((0.5f + arg0->x8_ptr1->light_color.b) - arg0->x2C_hex.b) / f;
    arg0->x4C_colorblend_alpha =
        ((0.5f + arg0->x8_ptr1->light_color.a) - arg0->x2C_hex.a) / f;
    ++arg0->x8_ptr1;
    return false;
}

bool lb_80014234(ColorOverlay* arg0)
{
    arg0->x7C_color_enable = false;
    ++arg0->x8_ptr1;
    return false;
}

u32 lbl_803BA1E8[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFF00, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000002, 0x00000000,
    0x00000000, 0x00000002, 0x00000000, 0x000000FF, 0x000000FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000002, 0x00000000,
};

lb_803BA248_fn lb_803BA248[] = {
    lb_80013BB0, lb_80013BB8, lb_80013BE4, lb_80013C18, lb_80013D68,
    lb_80013E3C, lb_80013F78, lb_80013FF0, lb_80014014, lb_800140F8,
    lb_80014234, NULL,        NULL,        NULL,        NULL,
    NULL,        NULL,        NULL,        NULL,        NULL,
    NULL,        NULL,
};

bool lb_80014258(HSD_GObj* gobj, void* arg1, FtCmd2 cmd)
{
    ColorOverlay* co = arg1;

    if (co->x8_ptr1 != NULL) {
        s32 timer = co->x0_timer;
        if (timer != 0) {
            co->x0_timer = timer - 1;
        }
    }

    while (co->x8_ptr1 != NULL && co->x0_timer == 0) {
        u32 opcode = co->x8_ptr1->unk.unk;
        if (!Command_Execute((CommandInfo*) co, opcode)) {
            if (opcode < 0x15U) {
                u32 idx = opcode - 0xA;
                if (lb_803BA248[idx](co)) {
                    return true;
                }
            } else {
                cmd(gobj, (CommandInfo*) co, (int) opcode);
            }
        }
    }

    if (co->x7C_color_enable) {
        co->x30_color_red += co->x40_colorblend_red;
        co->x34_color_green += co->x44_colorblend_green;
        co->x38_color_blue += co->x48_colorblend_blue;
        co->x3C_color_alpha += co->x4C_colorblend_alpha;
        co->x2C_hex.r = (u8) co->x30_color_red;
        co->x2C_hex.g = (u8) co->x34_color_green;
        co->x2C_hex.b = (u8) co->x38_color_blue;
        co->x2C_hex.a = (u8) co->x3C_color_alpha;
    }
    if (co->x7C_flag2) {
        co->x54_light_red += co->x64_lightblend_red;
        co->x58_light_green += co->x68_lightblend_green;
        co->x5C_light_blue += co->x6C_lightblend_blue;
        co->x60_light_alpha += co->x70_lightblend_alpha;
        co->x50_light_color.r = (u8) co->x54_light_red;
        co->x50_light_color.g = (u8) co->x58_light_green;
        co->x50_light_color.b = (u8) co->x5C_light_blue;
        co->x50_light_color.a = (u8) co->x60_light_alpha;
    }
    {
        s32 fc = co->x4_pri;
        if (fc != 0) {
            co->x4_pri = fc - 1;
            if (co->x4_pri == 0) {
                return true;
            }
        }
    }
    return false;
}

void lb_80014498(ColorOverlay* arg0)
{
    arg0->x8_ptr1 = NULL;
    arg0->x4_pri = 0;
    arg0->x28_colanim.ptr = NULL;
    arg0->x7C_color_enable = arg0->x7C_flag2 = false;
}

bool lb_800144C8(ColorOverlay* arg0, Fighter_804D653C_t* arg1, int arg2,
                 int arg3)
{
    if (arg1[arg0->x28_colanim.i].unk4 <= arg1[arg2].unk4) {
        arg0->x28_colanim.i = arg2;
        arg0->x4_pri = arg3;
        arg0->x8_ptr1 = arg1[arg2].unk;
        arg0->x0_timer = 0;
        arg0->xC_loop = 0;
        arg0->x7C_color_enable = arg0->x7C_flag2 = false;
        return true;
    }
    return false;
}

void lb_80014534(void)
{
    lbArchive_80017040(NULL, "LbRb.dat", &lb_804D63C0, "lbRumbleData", 0);
}

void lb_80014574(u8 arg0, int arg1, int arg2, int arg3)
{
    HSD_PadRumbleAdd(arg0, arg1, arg3 != 0 ? arg3 : -2, lb_804D63C0[arg2].unk4,
                     lb_804D63C0[arg2].unk);
}

void lb_800145C0(u8 slot)
{
    HSD_PadRumbleRemove(slot);
    HSD_PadRumbleOn(slot);
}

void lb_800145F4(void)
{
    int i;
    for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        lb_800145C0(i);
    }
}

bool lb_80014638(struct lb_80014638_arg0_t* arg0,
                 struct lb_80014638_arg1_t* arg1)
{
    Vec3 sp30, sp24, sp18;
    PAD_STACK(8);

    sp30 = arg0->x0;
    sp24 = arg0->xC;
    sp18 = arg1->x8;
    sp30.x = sp30.x + arg1->unk_x;
    sp30.y += arg1->unk_x;
    sp30.z += arg1->unk_y;
    if (sp30.z < sp24.z) {
        return false;
    }
    if (sp30.z < sp18.z || sp24.z > sp18.z) {
        return false;
    }
    {
        float z = sp18.z;
        if (((sp24.z - sp30.z) < 0.0F ? -(sp24.z - sp30.z) : sp24.z - sp30.z) <
            0.01F)
        {
            z = 1.0F;
        } else {
            z = (z - sp30.z) / (sp24.z - sp30.z);
        }
        if (z * (sp24.x - sp30.x) + sp30.x > sp18.y) {
            return false;
        }
        if (z * (sp24.y - sp30.y) + sp30.y < sp18.x) {
            return false;
        }
    }
    return true;
}

/* 4D3760 */ GXColor lb_ColorWhite = { 255, 255, 255, 255 };
/* 4D3764 */ GXColor lb_ColorBlack = { 0, 0, 0, 255 };
/* 4D3768 */ static GXColor red = { 255, 0, 0, 255 };
/* 4D376C */ static GXColor translucent_red = { 255, 0, 0, 64 };
/* 4D3770 */ static GXColor yellow = { 255, 255, 0, 255 };

bool lb_80014770(Vec3* arg0, int arg1)
{
    if ((u32) arg1 == 2U) {
        Vec3* near_pt;
        Vec3* far_pt;
        GXColor* near_clr;
        GXColor* far_clr;

        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                       GX_LO_NOOP);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetNumTexGens(0);
        GXSetTevClampMode(GX_TEVSTAGE0, 0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                      GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0,
                      GX_DF_NONE, GX_AF_NONE);
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        {
            MtxPtr mtx = HSD_CObjGetCurrent()->view_mtx;
            GXLoadPosMtxImm(mtx, 0);
        }
        GXSetCurrentMtx(0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 5);

        if (arg0[1].z > arg0[0].z) {
            near_pt = &arg0[1];
            far_pt = &arg0[0];
            near_clr = &red;
            far_clr = &translucent_red;
        } else {
            near_pt = &arg0[0];
            far_pt = &arg0[1];
            near_clr = &translucent_red;
            far_clr = &red;
        }

        GXPosition3f32(near_pt->y, near_pt->z, 0.0f);
        GXColor4u8(near_clr->r, near_clr->g, near_clr->b, near_clr->a);

        GXPosition3f32(near_pt->x, near_pt->z, 0.0f);
        GXColor4u8(near_clr->r, near_clr->g, near_clr->b, near_clr->a);

        GXPosition3f32(far_pt->y, far_pt->z, 0.0f);
        GXColor4u8(far_clr->r, far_clr->g, far_clr->b, far_clr->a);

        GXPosition3f32(near_pt->x, near_pt->z, 0.0f);
        GXColor4u8(near_clr->r, near_clr->g, near_clr->b, near_clr->a);

        GXPosition3f32(far_pt->x, far_pt->z, 0.0f);
        GXColor4u8(far_clr->r, far_clr->g, far_clr->b, far_clr->a);

        GXEnd();
        PAD_STACK(8);
        return true;
    }
    return false;
}

bool lb_800149E0(Mtx arg0, u32 arg1)
{
    PAD_STACK(16);
    if (arg1 == 0) {
        HSD_StateInitDirect(0, 2);
        {
            MtxPtr mtx = HSD_CObjGetCurrent()->view_mtx;
            GXLoadPosMtxImm(mtx, 0);
            GXSetLineWidth(12, GX_TO_ONE);
            GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
            {
                float y = arg0[1][0];
                GXPosition3f32(arg0[0][2], y, 0.0f);
                GXColor4u8(yellow.r, yellow.g, yellow.b, yellow.a);

                GXPosition3f32(arg0[0][3], y, 0.0f);
                GXColor4u8(yellow.r, yellow.g, yellow.b, yellow.a);
            }
            GXEnd();
        }
        return true;
    }
    return false;
}
