#ifndef FRAY_LB_LBOSD_H
#define FRAY_LB_LBOSD_H

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/types.h>
#include <stdarg.h>
#include <stddef.h>

typedef enum {
    Osd_AlignLeft = 0,
    Osd_AlignRight,
    Osd_AlignCenter,
} Osd_Align;

int snprintf(char* s, size_t n, const char* format, ...);

/// Create the panel. `id` must be unique across all DevText users.
/// `buf` must be at least `cols * rows * 2` bytes.
void Osd_Init(u32 id, u16 x, u16 y, u8 cols, u8 rows,
              f32 scale_x, f32 scale_y, char* buf);

/// Toggle visibility of the text and background.
void Osd_Show(void);
void Osd_Hide(void);

/// Colors. Defaults are opaque white on mostly-opaque black.
void Osd_SetBGColor(u8 r, u8 g, u8 b);
void Osd_SetTextColor(u8 r, u8 g, u8 b);
void Osd_SetAlpha(u8 alpha);

// ----------------------------------------------------------- page build

/// Clear the panel and reset the row cursor to the top.
void Osd_Begin(void);

/// Write a complete line at the current row, then advance.
/// Text is truncated if it exceeds the panel width.
void Osd_Text(const char* s);

void Osd_Fmt(const char* fmt, ...);

void Osd_RowBegin(void);
void Osd_CellStr(const char* s, size_t width);
void Osd_CellU32(u32 v, size_t width);
void Osd_CellF32(f32 v, size_t width, size_t decimals);
void Osd_CellVec3(const Vec3* v, size_t width, size_t decimals);

void Osd_RowEnd(void);

#endif
