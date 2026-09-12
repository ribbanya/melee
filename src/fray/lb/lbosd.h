#ifndef FRAY_LB_LBOSD_H
#define FRAY_LB_LBOSD_H

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/types.h>
#include <stdarg.h>
#include <stddef.h>

/// Thin wrapper around DevText for paginated tabular debug output.
///
/// The caller provides the cell buffer, sized `cols * rows * 2` bytes
/// (one char and one color byte per cell). The panel is created once
/// via Osd_Init and reused for every page.
///
/// Typical use:
///
///     static char buf[COLS * ROWS * 2];
///     Osd_Init(20, 4, 4, COLS, ROWS, buf);
///
///     // each frame or on page change:
///     Osd_Begin();
///     Osd_Fmt("frame=%u", frame);
///     Osd_Text("p s pos             vel");
///     Osd_RowBegin();
///     Osd_CellU32(p, 1);
///     Osd_CellU32(s, 1);
///     Osd_CellVec3(&f->pos, 7, 2);
///     Osd_CellVec3(&f->vel, 7, 3);
///     Osd_RowEnd();
///     Osd_Show();

// ----------------------------------------------------------- libc shim
//
// The MSL provides vsnprintf but not snprintf. This weak definition
// forwards to vsnprintf and guards the n == 0 case. A strong definition
// elsewhere takes precedence.

int snprintf(char* s, size_t n, const char* format, ...);

// ------------------------------------------------------------ lifecycle

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

/// Build a columnar row. Call RowBegin, one or more Cell*, then RowEnd.
/// Each cell is padded or truncated to exactly `width` characters,
/// followed by one space of separation.
void Osd_RowBegin(void);
void Osd_CellStr(const char* s, size_t width);
void Osd_CellU32(u32 v, size_t width);
void Osd_CellF32(f32 v, size_t width, size_t decimals);

/// Emit v->x, v->y, v->z as three consecutive cells with identical width
/// and decimal precision. Saves the caller from writing three lines per
/// vector, which is the most common row shape in snapshot dumps.
void Osd_CellVec3(const Vec3* v, size_t width, size_t decimals);

void Osd_RowEnd(void);

#endif
