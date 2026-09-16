#ifndef FRAY_LB_LBOSD_H
#define FRAY_LB_LBOSD_H

#include <stddef.h>

#include <dolphin/mtx.h>
#include <dolphin/types.h>

int snprintf(char* s, size_t n, const char* format, ...);

void Osd_Init(u32 id, int x, int y, size_t cols, size_t rows, f32 scale_x,
              f32 scale_y, char* buf);

void Osd_Show(void);
void Osd_Hide(void);
void Osd_SetAlpha(u8 alpha);
void Osd_SetBGColor(u8 r, u8 g, u8 b);
void Osd_SetTextColor(u8 r, u8 g, u8 b);

void Osd_Begin(void);
void Osd_PutStr(int x, int y, const char* s);
void Osd_PutFmt(int x, int y, const char* fmt, ...);
void Osd_PutU32(int x, int y, size_t width, u32 v);
void Osd_PutF32(int x, int y, size_t width, size_t decimals, f32 v);
void Osd_PutVec3(int x, int y, size_t width, size_t decimals, const Vec3* v);

#endif
