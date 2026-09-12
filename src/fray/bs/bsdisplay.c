#include <fray/bs/bsdisplay.h>
#include <fray/lb/lbosd.h>

#define OSD_ID 20
#define OSD_COLS 80
#define OSD_ROWS 34
#define OSD_GLYPH_W 8
#define OSD_GLYPH_H 14

typedef struct {
    const Bisim_GlobalSnapshot* g;
    const Bisim_PlayerSnapshot* p;
    const Bisim_FighterSnapshot* f;
    size_t port;
    size_t sub;
} Row;

typedef void (*CellFn)(const Row* r, size_t width);

typedef struct {
    const char* header;
    size_t width;
    CellFn cell;
} Column;

typedef struct {
    const char* title;
    const Column* columns;
} Page;

static void cellPort(const Row* r, size_t width)
{
    Osd_CellU32((u32) r->port, width);
}

static void cellSub(const Row* r, size_t width)
{
    Osd_CellU32((u32) r->sub, width);
}

static void cellMsid(const Row* r, size_t width)
{
    Osd_CellU32((u32) r->f->msid, width);
}

static void cellAnim(const Row* r, size_t width)
{
    Osd_CellF32(r->f->anim_frame, width, 2);
}

static void cellAirborne(const Row* r, size_t width)
{
    Osd_CellU32((u32) r->f->airborne, width);
}

static void cellFacing(const Row* r, size_t width)
{
    Osd_CellF32(r->f->facing_dir, width, 0);
}

static void cellPosX(const Row* r, size_t width)
{
    Osd_CellF32(r->f->pos.x, width, 2);
}
static void cellPosY(const Row* r, size_t width)
{
    Osd_CellF32(r->f->pos.y, width, 2);
}
static void cellPosZ(const Row* r, size_t width)
{
    Osd_CellF32(r->f->pos.z, width, 2);
}

static void cellVelX(const Row* r, size_t width)
{
    Osd_CellF32(r->f->vel.x, width, 3);
}
static void cellVelY(const Row* r, size_t width)
{
    Osd_CellF32(r->f->vel.y, width, 3);
}
static void cellVelZ(const Row* r, size_t width)
{
    Osd_CellF32(r->f->vel.z, width, 3);
}

static void cellAccelX(const Row* r, size_t width)
{
    Osd_CellF32(r->f->accel.x, width, 3);
}
static void cellAccelY(const Row* r, size_t width)
{
    Osd_CellF32(r->f->accel.y, width, 3);
}
static void cellAccelZ(const Row* r, size_t width)
{
    Osd_CellF32(r->f->accel.z, width, 3);
}

static void cellKbVelX(const Row* r, size_t width)
{
    Osd_CellF32(r->f->kb_vel.x, width, 3);
}
static void cellKbVelY(const Row* r, size_t width)
{
    Osd_CellF32(r->f->kb_vel.y, width, 3);
}
static void cellKbVelZ(const Row* r, size_t width)
{
    Osd_CellF32(r->f->kb_vel.z, width, 3);
}

static char panel_buf[OSD_COLS * OSD_ROWS * 2];

static const Column colsState[] = {
    { "p", 1, cellPort },       { "s", 1, cellSub },
    { "msid", 4, cellMsid },    { "anim", 6, cellAnim },
    { "air", 3, cellAirborne }, { "face", 4, cellFacing },
    { NULL, 0, NULL },
};

static const Column colsPos[] = {
    { "p", 1, cellPort },  { "s", 1, cellSub },   { "px", 7, cellPosX },
    { "py", 7, cellPosY }, { "pz", 7, cellPosZ }, { NULL, 0, NULL },
};

static const Column colsVel[] = {
    { "p", 1, cellPort },  { "s", 1, cellSub },   { "vx", 7, cellVelX },
    { "vy", 7, cellVelY }, { "vz", 7, cellVelZ }, { NULL, 0, NULL },
};

static const Column colsAccel[] = {
    { "p", 1, cellPort },    { "s", 1, cellSub },     { "ax", 7, cellAccelX },
    { "ay", 7, cellAccelY }, { "az", 7, cellAccelZ }, { NULL, 0, NULL },
};

static const Column colsKbVel[] = {
    { "p", 1, cellPort },    { "s", 1, cellSub },     { "kx", 7, cellKbVelX },
    { "ky", 7, cellKbVelY }, { "kz", 7, cellKbVelZ }, { NULL, 0, NULL },
};

static const Page pages[] = {
    { "state", colsState }, { "pos", colsPos },  { "vel", colsVel },
    { "acc", colsAccel },   { "kb", colsKbVel },
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
    for (c = page->columns; c->header != NULL; c++) {
        Osd_CellStr(c->header, c->width);
    }
    Osd_RowEnd();

    for (p = 0; p < GM_MAX_PLAYERS; p++) {
        const Bisim_PlayerSnapshot* ps = &g->players[p];
        if (!ps->exists) {
            continue;
        }

        for (s = 0; s < PL_MAX_SUB_FIGHTERS; s++) {
            const Bisim_FighterSnapshot* fs = &ps->fighters[s];
            Row r;

            if (!fs->exists) {
                continue;
            }

            r.g = g;
            r.p = ps;
            r.f = fs;
            r.port = p;
            r.sub = s;

            Osd_RowBegin();
            for (c = page->columns; c->header != NULL; c++) {
                c->cell(&r, c->width);
            }
            Osd_RowEnd();
        }
    }
}
