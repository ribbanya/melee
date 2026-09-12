#include <fray/bs/bsdisplay.h>
#include <fray/lb/lbosd.h>

#define OSD_ID 20
#define OSD_COLS 80
#define OSD_ROWS 34
#define OSD_GLYPH_W 8
#define OSD_GLYPH_H 14

typedef struct {
    const char* label;
    size_t width;
} Column;

typedef void (*EmitFn)(const Bisim_GlobalSnapshot* g,
                       const Bisim_PlayerSnapshot* p,
                       const Bisim_FighterSnapshot* f);

typedef struct {
    const char* title;
    const Column* columns; // { NULL, 0 } terminated
    EmitFn emit;
} Page;

static void emitState(UNUSED const Bisim_GlobalSnapshot* g,
                      UNUSED const Bisim_PlayerSnapshot* p,
                      const Bisim_FighterSnapshot* f)
{
    Osd_CellU32(f->msid, 4);
    Osd_CellF32(f->anim_frame, 6, 2);
    Osd_CellU32(f->airborne, 3);
    Osd_CellF32(f->facing_dir, 4, 0);
}

static void emitPos(UNUSED const Bisim_GlobalSnapshot* g,
                    UNUSED const Bisim_PlayerSnapshot* p,
                    const Bisim_FighterSnapshot* f)
{
    Osd_CellVec3(&f->pos, 7, 2);
}

static void emitVel(UNUSED const Bisim_GlobalSnapshot* g,
                    UNUSED const Bisim_PlayerSnapshot* p,
                    const Bisim_FighterSnapshot* f)
{
    Osd_CellVec3(&f->vel, 7, 3);
}

static void emitAccel(UNUSED const Bisim_GlobalSnapshot* g,
                      UNUSED const Bisim_PlayerSnapshot* p,
                      const Bisim_FighterSnapshot* f)
{
    Osd_CellVec3(&f->accel, 7, 3);
}

static void emitKbVel(UNUSED const Bisim_GlobalSnapshot* g,
                      UNUSED const Bisim_PlayerSnapshot* p,
                      const Bisim_FighterSnapshot* f)
{
    Osd_CellVec3(&f->kb_vel, 7, 3);
}

static char panel_buf[OSD_COLS * OSD_ROWS * 2];

static const Column colsState[] = {
    { "p", 1 },   { "s", 1 },    { "msid", 4 }, { "anim", 7 },
    { "air", 3 }, { "face", 4 }, { NULL, 0 },
};

static const Column colsPos[] = {
    { "p", 1 }, { "s", 1 }, { "px", 7 }, { "py", 7 }, { "pz", 7 }, { NULL, 0 },
};

static const Column colsVel[] = {
    { "p", 1 }, { "s", 1 }, { "vx", 7 }, { "vy", 7 }, { "vz", 7 }, { NULL, 0 },
};

static const Column colsAccel[] = {
    { "p", 1 }, { "s", 1 }, { "ax", 7 }, { "ay", 7 }, { "az", 7 }, { NULL, 0 },
};

static const Column colsKbVel[] = {
    { "p", 1 }, { "s", 1 }, { "kx", 7 }, { "ky", 7 }, { "kz", 7 }, { NULL, 0 },
};

static const Page pages[] = {
    { "state", colsState, emitState }, { "pos", colsPos, emitPos },
    { "vel", colsVel, emitVel },       { "acc", colsAccel, emitAccel },
    { "kb", colsKbVel, emitKbVel },
};

static size_t page_idx = 0;

void BsDisplay_Init(void)
{
    Osd_Init(OSD_ID, 0, 0, OSD_COLS, OSD_ROWS, OSD_GLYPH_W, OSD_GLYPH_H,
             panel_buf);
    page_idx = 0;
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
    page_idx = (page_idx + 1) % ARRAY_SIZE(pages);
}

void BsDisplay_Draw(const Bisim_GlobalSnapshot* g)
{
    const Page* page = &pages[page_idx];
    const Column* c;
    size_t p;
    size_t s;

    Osd_Begin();

    Osd_Fmt("frame=%u seed=%08X  %s", g->curr_frame, g->seed, page->title);

    Osd_RowBegin();
    for (c = page->columns; c->label != NULL; c++) {
        Osd_CellStr(c->label, c->width);
    }
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

            Osd_RowBegin();
            Osd_CellU32(p, 1);
            Osd_CellU32(s, 1);
            page->emit(g, ps, fs);
            Osd_RowEnd();
        }
    }
}
