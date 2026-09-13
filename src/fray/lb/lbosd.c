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

static DevText* text;
static char* cells;
static size_t cols;
static size_t rows;

static GXColor bg_color = { 0x00, 0x00, 0x00, 0x00 };
static GXColor fg_color = { 0xFF, 0xFF, 0xFF, 0x00 };

static void applyColors(void)
{
    if (!text) {
        return;
    }

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

static void setColor(GXColor* dst, u8 r, u8 g, u8 b)
{
    dst->r = r;
    dst->g = g;
    dst->b = b;
    applyColors();
}

void Osd_Init(u32 id, int x, int y, size_t n_cols, size_t n_rows, f32 scale_x,
              f32 scale_y, char* buf)
{
    cols = n_cols;
    rows = n_rows;
    cells = buf;

    text = DevText_Create(id, x, y, n_cols, n_rows, buf);
    FRAY_ASSERT(text != NULL);

    DevText_Show(DevText_GetGObj(), text);
    DevText_HideCursor(text);
    DevText_SetScale(text, scale_x, scale_y);
    applyColors();
}

void Osd_Show(void)
{
    Osd_SetAlpha(0xFF);
}

void Osd_Hide(void)
{
    Osd_SetAlpha(0x00);
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
}

static void putChar(int x, int y, char c)
{
    if (x < 0 || y < 0) {
        return;
    }
    if ((size_t) x >= cols || (size_t) y >= rows) {
        return;
    }
    cells[(y * cols + x) * 2] = c;
}

void Osd_PutStr(int x, int y, const char* s)
{
    while (*s) {
        putChar(x, y, *s);
        s++;
        x++;
    }
}

void Osd_PutFmt(int x, int y, const char* fmt, ...)
{
    char tmp[256];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);

    Osd_PutStr(x, y, tmp);
}

static void putRight(int x, int y, size_t width, const char* s)
{
    size_t n = strlen(s);
    int start;

    if (n >= width) {
        start = x;
    } else {
        start = x + (int) (width - n);
    }

    Osd_PutStr(start, y, s);
}

void Osd_PutU32(int x, int y, size_t width, u32 v)
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%u", v);
    putRight(x, y, width, tmp);
}

void Osd_PutF32(int x, int y, size_t width, size_t decimals, f32 v)
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
    putRight(x, y, width, tmp);
}

void Osd_PutVec3(int x, int y, size_t width, size_t decimals, const Vec3* v)
{
    Osd_PutF32(x, y, width, decimals, v->x);
    Osd_PutF32(x + (int) (width + 1), y, width, decimals, v->y);
    Osd_PutF32(x + 2 * (int) (width + 1), y, width, decimals, v->z);
}
