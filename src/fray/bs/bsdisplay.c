#include <fray/bs/bsdisplay.h>
#include <fray/lb/lbosd.h>

#define OSD_ID 20
#define OSD_COLS 80
#define OSD_ROWS 34
#define OSD_GLYPH_W 8.0f
#define OSD_GLYPH_H 14.0f

typedef enum {
    BsLayer_State = 0,
    BsLayer_Pos,
    BsLayer_Vel,
    BsLayer_Accel,
    BsLayer_KbVel,
    BsLayer_Count,
} BsLayer;

typedef enum {
    BsLayerMask_State = 1 << BsLayer_State,
    BsLayerMask_Pos = 1 << BsLayer_Pos,
    BsLayerMask_Vel = 1 << BsLayer_Vel,
    BsLayerMask_Accel = 1 << BsLayer_Accel,
    BsLayerMask_KbVel = 1 << BsLayer_KbVel,
    BsLayerMask_All = BsLayerMask_State | BsLayerMask_Pos | BsLayerMask_Vel |
                      BsLayerMask_Accel | BsLayerMask_KbVel,
} BsLayerMask;

static char panel_buf[OSD_COLS * OSD_ROWS * 2];

static u32 layersForFighterCount(size_t n)
{
    u32 mask = BsLayerMask_All;

    if (n > 4) {
        mask &= ~BsLayerMask_KbVel;
    }
    if (n > 6) {
        mask &= ~BsLayerMask_Accel;
    }
    if (n > 8) {
        mask &= ~BsLayerMask_Vel;
    }

    return mask;
}

void BsDisplay_Init(void)
{
    Osd_Init(OSD_ID, 0, 0, OSD_COLS, OSD_ROWS, OSD_GLYPH_W, OSD_GLYPH_H,
             panel_buf);
}

void BsDisplay_Show(void)
{
    Osd_Show();
}

void BsDisplay_Hide(void)
{
    Osd_Hide();
}

void BsDisplay_Draw(const Bisim_GlobalSnapshot* g)
{
    size_t n = 0;
    size_t p;
    size_t s;

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &g->players[p];
        if (!ps->exists) {
            continue;
        }
        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            if (ps->fighters[s].exists) {
                n++;
            }
        }
    }

    Osd_SetLayers(layersForFighterCount(n));
    Osd_Begin();

    Osd_Line(U32_MAX, "frame=%u seed=%08X", g->curr_frame, g->seed);

    Osd_RowBegin(BsLayerMask_State);
    Osd_CellStr("p", 1);
    Osd_CellStr("s", 1);
    Osd_CellStr("msid", 4);
    Osd_CellStr("anim", 6);
    Osd_CellStr("air", 3);
    Osd_CellStr("face", 4);
    Osd_RowEnd();

    Osd_RowBegin(BsLayerMask_Pos);
    Osd_CellStr("p", 1);
    Osd_CellStr("s", 1);
    Osd_CellStr("px", 7);
    Osd_CellStr("py", 7);
    Osd_CellStr("pz", 7);
    Osd_RowEnd();

    Osd_RowBegin(BsLayerMask_Vel);
    Osd_CellStr("p", 1);
    Osd_CellStr("s", 1);
    Osd_CellStr("vx", 7);
    Osd_CellStr("vy", 7);
    Osd_CellStr("vz", 7);
    Osd_RowEnd();

    Osd_RowBegin(BsLayerMask_Accel);
    Osd_CellStr("p", 1);
    Osd_CellStr("s", 1);
    Osd_CellStr("ax", 7);
    Osd_CellStr("ay", 7);
    Osd_CellStr("az", 7);
    Osd_RowEnd();

    Osd_RowBegin(BsLayerMask_KbVel);
    Osd_CellStr("p", 1);
    Osd_CellStr("s", 1);
    Osd_CellStr("kx", 7);
    Osd_CellStr("ky", 7);
    Osd_CellStr("kz", 7);
    Osd_RowEnd();

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &g->players[p];
        if (!ps->exists) {
            continue;
        }

        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            const Bisim_FighterSnapshot* fs = &ps->fighters[s];
            if (!fs->exists) {
                continue;
            }

            Osd_RowBegin(BsLayerMask_State);
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            Osd_CellU32((u32) fs->msid, 4);
            Osd_CellF32(fs->anim_frame, 6, 2);
            Osd_CellU32((u32) fs->airborne, 3);
            Osd_CellF32(fs->facing_dir, 4, 0);
            Osd_RowEnd();

            Osd_RowBegin(BsLayerMask_Pos);
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            Osd_CellVec3(&fs->pos, 7, 2);
            Osd_RowEnd();

            Osd_RowBegin(BsLayerMask_Vel);
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            Osd_CellVec3(&fs->vel, 7, 3);
            Osd_RowEnd();

            Osd_RowBegin(BsLayerMask_Accel);
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            Osd_CellVec3(&fs->accel, 7, 3);
            Osd_RowEnd();

            Osd_RowBegin(BsLayerMask_KbVel);
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            Osd_CellVec3(&fs->kb_vel, 7, 3);
            Osd_RowEnd();
        }
    }
}
