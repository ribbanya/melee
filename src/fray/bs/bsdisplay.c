#include <fray/bs/bsdisplay.h>
#include <fray/lb/lbosd.h>

#define OSD_COLS 56
#define OSD_ROWS 14
#define OSD_ID 20
#define OSD_X 4
#define OSD_Y 4

static char panel_buf[OSD_COLS * OSD_ROWS * 2];

typedef void (*EmitFn)(const Bisim_GlobalSnapshot* g,
                       const Bisim_PlayerSnapshot* p,
                       const Bisim_FighterSnapshot* f);

typedef struct {
    const char* title;
    const char* header;
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

static const Page pages[] = {
    { "state", "p s msid anim   air face", emitState },
    { "pos", "p s px     py     pz", emitPos },
    { "vel", "p s vx     vy     vz", emitVel },
    { "acc", "p s ax     ay     az", emitAccel },
    { "kb", "p s kx     ky     kz", emitKbVel },
};

#define NUM_PAGES (ARRAY_SIZE(pages))

static size_t page_idx = 0;

void BsDisplay_Init(void)
{
    Osd_Init(OSD_ID, OSD_X, OSD_Y, OSD_COLS, OSD_ROWS, panel_buf);
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
    page_idx = (page_idx + 1) % NUM_PAGES;
}

void BsDisplay_Draw(const Bisim_GlobalSnapshot* g)
{
    const Page* page = &pages[page_idx];
    size_t p;
    size_t s;

    Osd_Begin();

    Osd_Fmt("frame=%u seed=%08X  %s", g->curr_frame, g->seed, page->title);
    Osd_Text(page->header);

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
