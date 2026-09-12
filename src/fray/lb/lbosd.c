#include <printf.h> // IWYU pragma: keep
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "fray/lb/lbqol.h"
#include "melee/if/textdraw.h"
#include <fray/lb/lbosd.h>
#include <melee/if/textlib.h>

// One instance. Multiple panels are possible but unused.
static DevText* text;
static u8 cols;
static u8 rows;

// Per-row scratch. Line width is bounded by the panel width.
static char line[256];
static size_t row;
static size_t col;
static GXColor bg_color = { 0x00, 0x00, 0x00, 0x00 };
static GXColor fg_color = { 0xFF, 0xFF, 0xFF, 0x00 };

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

static void applyColors(void)
{
    FRAY_ASSERT(text != NULL);

    if (bg_color.a == 0) {
        DevText_HideText(text);
        DevText_HideBackground(text);
        return;
    }

    DevText_SetBGColor(text, bg_color);
    DevText_SetTextColor(text, fg_color);
    DevText_ShowText(text);
    DevText_ShowBackground(text);
}

void Osd_Init(u32 id, u16 x, u16 y, u8 n_cols, u8 n_rows, f32 scale_x,
              f32 scale_y, char* buf)
{
    cols = n_cols;
    rows = n_rows;
    row = 0;
    col = 0;

    text = DevText_Create(id, x, y, n_cols, n_rows, buf);
    FRAY_ASSERT(text != NULL);
    DevText_Show(DevText_GetGObj(), text);
    DevText_HideCursor(text);
    DevText_SetScale(text, scale_x, scale_y);
    applyColors();
}

void Osd_Hide(void)
{
    Osd_SetAlpha(0x00);
}

void Osd_Show(void)
{
    Osd_SetAlpha(0xFF);
}

static void setColor(GXColor* dst, u8 r, u8 g, u8 b)
{
    dst->r = r;
    dst->g = g;
    dst->b = b;
    applyColors();
}

void Osd_SetAlpha(u8 alpha)
{
    bg_color.a = alpha;
    fg_color.a = alpha;
    applyColors();
}

void Osd_SetBGColor(u8 r, u8 g, u8 b)
{
    setColor(&bg_color, r, g, b);
}

void Osd_SetTextColor(u8 r, u8 g, u8 b)
{
    setColor(&fg_color, r, g, b);
}

void Osd_Begin(void)
{
    FRAY_ASSERT(text != NULL);
    DevText_Erase(text);
    row = 0;
    col = 0;
    line[0] = '\0';
}

static void flushRow(void)
{
    FRAY_ASSERT(text != NULL);
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

void Osd_Text(const char* s)
{
    size_t n;

    FRAY_ASSERT(text != NULL);

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

    FRAY_ASSERT(text != NULL);

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

void Osd_RowBegin(void)
{
    col = 0;
    line[0] = '\0';
}

static void cellPut(const char* s, size_t width)
{
    size_t n;
    size_t pad;
    size_t i;

    FRAY_ASSERT(text != NULL);
    if (width == 0) {
        return;
    }

    if (col + width + 1 > cols) {
        return;
    }

    n = strlen(s);
    if (n > width) {
        n = width;
    }
    pad = width - n;

    for (i = 0; i < pad; i++) {
        line[col++] = ' ';
    }
    memcpy(&line[col], s, n);
    col += n;

    line[col++] = ' '; // separator
}

void Osd_CellStr(const char* s, size_t width)
{
    cellPut(s, width);
}

void Osd_CellU32(u32 v, size_t width)
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u", v);
    cellPut(tmp, width);
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
    cellPut(tmp, width);
}

void Osd_RowEnd(void)
{
    flushRow();
}
