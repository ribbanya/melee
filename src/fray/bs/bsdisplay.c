#include "melee/pl/forward.h"
#include <fray/bs/bsdisplay.h>
#include <fray/lb/lbosd.h>

#define OSD_ID 20
#define OSD_COLS 54
#define OSD_ROWS 34
#define OSD_GLYPH_W 10
#define OSD_GLYPH_H 14

typedef enum {
    BsLayer_Global = 0,
    BsLayer_Players,
    BsLayer_State,
    BsLayer_Dynamics,
    BsLayer_Impulse,
    BsLayer_Count,
} BsLayer;

typedef enum {
    BsLayerMask_Global = 1 << BsLayer_Global,
    BsLayerMask_Players = 1 << BsLayer_Players,
    BsLayerMask_State = 1 << BsLayer_State,
    BsLayerMask_Dynamics = 1 << BsLayer_Dynamics,
    BsLayerMask_Impulse = 1 << BsLayer_Impulse,
    BsLayerMask_All = BsLayerMask_Global | BsLayerMask_Players |
                      BsLayerMask_State | BsLayerMask_Dynamics |
                      BsLayerMask_Impulse,
} BsLayerMask;

typedef struct {
    int x;
    size_t w;
} RowCol;

static const RowCol playerRow[] = {
    { 0, 1 },
    { 2, 3 },
    { 6, 1 },
};

static const RowCol stateRow[] = {
    { 0, 1 }, { 2, 1 }, { 4, 4 }, { 9, 8 }, { 20, 1 }, { 22, 1 },
};

static const RowCol vecRow[] = {
    { 0, 1 },  { 2, 1 },  { 4, 7 },  { 12, 7 },
    { 20, 7 }, { 28, 7 }, { 36, 7 }, { 44, 7 },
};

static char panel_buf[OSD_COLS * OSD_ROWS * 2];

static const Bisim_GlobalSnapshot* snapshot;
static const RowCol* row;
static u32 layers;
static int y;
static int col;
static int page;
static int pages;

static const char* pkindName(Gm_PKind pkind)
{
    switch (pkind) {
    case Gm_PKind_Human:
        return "hmn";
    case Gm_PKind_Cpu:
        return "cpu";
    case Gm_PKind_Demo:
        return "dem";
    case Gm_PKind_NA:
        return "na";
    case Gm_PKind_Boss:
        return "bos";
    default:
        return "unk";
    }
}

static size_t countFighters(void)
{
    size_t n = 0, i, j;

    for (i = 0; i < GM_MAX_PLAYERS; i++) {
        const Bisim_PlayerSnapshot* ps = &snapshot->players[i];
        if (!ps->exists) {
            continue;
        }
        for (j = 0; j < PL_MAX_SUB_FIGHTERS; j++) {
            if (ps->fighters[j].exists) {
                n++;
            }
        }
    }
    return n;
}

static u32 layersForFighterCount(size_t n)
{
    u32 mask = BsLayerMask_All;

    if (n > 4) {
        mask &= ~BsLayerMask_Impulse;
    }
    if (n > 8) {
        mask &= ~BsLayerMask_Dynamics;
    }

    return mask;
}

static void putStr(const char* s)
{
    Osd_PutStr(row[col].x, y, s);
    col++;
}

static void putU32(u32 v)
{
    Osd_PutU32(row[col].x, y, row[col].w, v);
    col++;
}

static void putF32(f32 v, size_t decimals)
{
    Osd_PutF32(row[col].x, y, row[col].w, decimals, v);
    col++;
}

static void putVec(const Vec3* v, size_t decimals)
{
    putF32(v->x, decimals);
    putF32(v->y, decimals);
    putF32(v->z, decimals);
}

static void putVec3Header(const char* prefix)
{
    char label[8];
    size_t n = strlen(prefix);
    FRAY_ASSERT(n < sizeof(label));

    memcpy(label, prefix, n);
    label[n + 1] = '\0';

    label[n] = 'x';
    putStr(label);
    label[n] = 'y';
    putStr(label);
    label[n] = 'z';
    putStr(label);
}

static void drawGlobal(void)
{
    if (!(layers & BsLayerMask_Global)) {
        return;
    }

    Osd_PutFmt(0, y, "frame=%u seed=%08X hash=%08X", snapshot->curr_frame,
               snapshot->seed, 0);
    y++;
}

static void drawPlayers(void)
{
    size_t p;

    if (!(layers & BsLayerMask_Players)) {
        return;
    }

    row = playerRow;
    col = 0;
    putStr("p");
    putStr("pk");
    putStr("c");
    y++;

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &snapshot->players[p];
        if (!ps->exists) {
            continue;
        }

        col = 0;
        putU32(p);
        putStr(pkindName(ps->pkind));
        putU32(ps->color);
        y++;
    }
}

static void drawState(void)
{
    static const size_t dec_anim = 3;
    static const size_t dec_face = 0;
    size_t p;
    size_t s;

    if (!(layers & BsLayerMask_State)) {
        return;
    }

    row = stateRow;
    col = 0;
    putStr("p");
    putStr("s");
    putStr("msid");
    putStr("anim");
    putStr("a");
    putStr("f");
    y++;

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &snapshot->players[p];
        if (!ps->exists) {
            continue;
        }

        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            const Bisim_FighterSnapshot* fs = &ps->fighters[s];
            if (!fs->exists) {
                continue;
            }

            col = 0;
            putU32(p);
            putU32(s);
            putU32((u32) fs->msid);
            putF32(fs->anim_frame, dec_anim);
            putU32((u32) fs->airborne);
            putF32(fs->facing_dir, dec_face);
            y++;
        }
    }
}

static void drawDynamics(void)
{
    static const size_t decimals = 2;
    size_t p;
    size_t s;

    if (!(layers & BsLayerMask_Dynamics)) {
        return;
    }

    row = vecRow;
    col = 0;
    putStr("p");
    putStr("s");
    putVec3Header("p");
    putVec3Header("v");
    y++;

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &snapshot->players[p];
        if (!ps->exists) {
            continue;
        }

        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            const Bisim_FighterSnapshot* fs = &ps->fighters[s];
            if (!fs->exists) {
                continue;
            }

            col = 0;
            putU32(p);
            putU32(s);
            putVec(&fs->pos, decimals);
            putVec(&fs->vel, decimals);
            y++;
        }
    }
}

static void drawImpulse(void)
{
    static const size_t decimals = 2;
    size_t p;
    size_t s;

    if (!(layers & BsLayerMask_Impulse)) {
        return;
    }

    row = vecRow;
    col = 0;
    putStr("p");
    putStr("s");
    putVec3Header("a");
    putVec3Header("k");
    y++;

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &snapshot->players[p];
        if (!ps->exists) {
            continue;
        }

        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            const Bisim_FighterSnapshot* fs = &ps->fighters[s];
            if (!fs->exists) {
                continue;
            }

            col = 0;
            putU32(p);
            putU32(s);
            putVec(&fs->accel, decimals);
            putVec(&fs->kb_vel, decimals);
            y++;
        }
    }
}

void BsDisplay_Init(void)
{
    Osd_Init(OSD_ID, 0, 0, OSD_COLS, OSD_ROWS, OSD_GLYPH_W, OSD_GLYPH_H,
             panel_buf);
    page = 0;
    pages = 1;
}

void BsDisplay_Show(void)
{
    Osd_Show();
}
void BsDisplay_Hide(void)
{
    Osd_Hide();
}

void BsDisplay_NextPage(void)
{
    page = (page + 1) % pages;
}

void BsDisplay_Draw(const Bisim_GlobalSnapshot* g)
{
    snapshot = g;
    layers = layersForFighterCount(countFighters());
    y = 0;

    Osd_Begin();
    drawGlobal();
    drawPlayers();
    drawState();
    drawDynamics();
    drawImpulse();
}
