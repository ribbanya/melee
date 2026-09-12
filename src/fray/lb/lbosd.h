#ifndef FRAY_LB_LBOSD_H
#define FRAY_LB_LBOSD_H

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/types.h>
#include <stdarg.h>
#include <stddef.h>

 int snprintf(char* s, size_t n, const char* format, ...);

void Osd_Init(u32 id, u16 x, u16 y, u8 cols, u8 rows,
              f32 scale_x, f32 scale_y, char* buf);

void Osd_Show(void);
void Osd_Hide(void);
void Osd_SetAlpha(u8 alpha);
void Osd_SetBGColor(u8 r, u8 g, u8 b);
void Osd_SetTextColor(u8 r, u8 g, u8 b);

void Osd_SetLayers(u32 layers);

void Osd_Begin(void);
void Osd_Line(u32 layers, const char* fmt, ...);
void Osd_RowBegin(u32 layers);
void Osd_CellStr(const char* s, size_t width);
void Osd_CellU32(u32 v, size_t width);
void Osd_CellF32(f32 v, size_t width, size_t decimals);
void Osd_CellVec3(const Vec3* v, size_t width, size_t decimals);
void Osd_RowEnd(void);
void Osd_Ruler(void);

#endif
