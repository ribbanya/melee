#include <printf.h> // IWYU pragma: keep
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "fray/lb/lbqol.h"
#include "melee/if/textdraw.h"
#include <fray/lb/lbosd.h>
#include <melee/if/textlib.h>

int snprintf(char* s, size_t n, const char* format, ...)
{
    va_list ap;
    int r;

    va_start(ap, format);

    if (n == 0) {
        char scratch[1];
        r = vsnprintf(scratch, 1, format, ap);
    } else {
        r = vsnprintf(s, n, format, ap);
    }

    va_end(ap);
    return r;
}

// One instance. Multiple panels are possible but unused.
static DevText* text;
static u8 cols;
static u8 rows;

// Per-row scratch. Line width is bounded by the panel width.
static char line[256];
static size_t row;
static size_t col;

// ---------------------------------------------------------------- Setup

void Osd_Init(u32 id, u16 x, u16 y, u8 n_cols, u8 n_rows, char* buf)
{
    GXColor bg = { 0xFF, 0x00, 0xFF, 0xFF };
    GXColor fg = { 0xFF, 0xFF, 0xFF, 0xFF };

    cols = n_cols;
    rows = n_rows;
    row = 0;
    col = 0;

    text = DevText_Create(id, x, y, n_cols, n_rows, buf);
    FRAY_ASSERT(text);

    DevText_Show(NULL, text);
    DevText_ShowText(text);
    DevText_HideCursor(text);
    DevText_SetBGColor(text, bg);
    DevText_SetTextColor(text, fg);
    DevText_SetScale(text, 10, 14);
    DevText_ShowBackground(text);
    DevText_HideText(text);
}

void Osd_Show(void)
{
    FRAY_ASSERT(text);
    OSReport("osd show, text=%p\n", text);
    DevText_ShowText(text);
    DevText_ShowBackground(text);
    OSReport("osd show done\n");
}

void Osd_Hide(void)
{
    FRAY_ASSERT(text);
    DevText_HideText(text);
    DevText_HideBackground(text);
}

void Osd_SetBGColor(GXColor c)
{
    FRAY_ASSERT(text);
    DevText_SetBGColor(text, c);
}

void Osd_SetTextColor(GXColor c)
{
    FRAY_ASSERT(text);
    DevText_SetTextColor(text, c);
}

// ------------------------------------------------------------ Row state

void Osd_Begin(void)
{
    FRAY_ASSERT(text);
    DevText_Erase(text);
    row = 0;
    col = 0;
    line[0] = '\0';
}

static void flushRow(void)
{
    FRAY_ASSERT(text);
    if (row >= rows) {
        return;
    }

    line[col] = '\0';
    DevText_SetCursorXY(text, 0, row);
    DevText_Print(text, line);

    row++;
    col = 0;
    line[0] = '\0';
}

// -------------------------------------------------------- Line writers

void Osd_Text(const char* s)
{
    size_t n;

    FRAY_ASSERT(text);

    n = strlen(s);
    if (n > cols) {
        n = cols;
    }

    memcpy(line, s, n);
    col = n;
    flushRow();
}

void Osd_Fmt(const char* fmt, ...)
{
    va_list ap;
    size_t n;

    FRAY_ASSERT(text);

    va_start(ap, fmt);
    vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);

    n = strlen(line);
    if (n > cols) {
        n = cols;
    }
    col = n;
    flushRow();
}

// -------------------------------------------------------- Columnar row

void Osd_RowBegin(void)
{
    col = 0;
    line[0] = '\0';
}

// Write `s` padded or truncated to exactly `width` characters, then a space.
// `right_align` = 0: left-justified. 1: right-justified.
static void cellPut(const char* s, size_t width, int right_align)
{
    size_t n;
    size_t pad;
    size_t i;

    FRAY_ASSERT(text);
    if (width == 0) {
        return;
    }

    // Would the cell plus a separator fit on this row?
    if (col + width + 1 > cols) {
        return;
    }

    n = strlen(s);
    if (n > width) {
        n = width;
    }
    pad = width - n;

    if (right_align) {
        for (i = 0; i < pad; i++) {
            line[col++] = ' ';
        }
        memcpy(&line[col], s, n);
        col += n;
    } else {
        memcpy(&line[col], s, n);
        col += n;
        for (i = 0; i < pad; i++) {
            line[col++] = ' ';
        }
    }

    line[col++] = ' '; // separator
}

void Osd_CellStr(const char* s, size_t width)
{
    cellPut(s, width, 0);
}

void Osd_CellU32(u32 v, size_t width)
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u", v);
    cellPut(tmp, width, 1);
}

void Osd_CellF32(f32 v, size_t width, size_t decimals)
{
    char fmt[8];
    char tmp[32];

    if (decimals > 6) {
        decimals = 6;
    }

    fmt[0] = '%';
    fmt[1] = '.';
    fmt[2] = (char) ('0' + decimals);
    fmt[3] = 'f';
    fmt[4] = '\0';

    snprintf(tmp, sizeof(tmp), fmt, (double) v);
    cellPut(tmp, width, 1);
}

void Osd_CellVec3(const Vec3* v, size_t width, size_t decimals)
{
    Osd_CellF32(v->x, width, decimals);
    Osd_CellF32(v->y, width, decimals);
    Osd_CellF32(v->z, width, decimals);
}

void Osd_RowEnd(void)
{
    flushRow();
}
