#ifndef FRAY_BS_BSHASH_H
#define FRAY_BS_BSHASH_H

#include <Runtime/platform.h>

#include <dolphin/mtx.h>

u32 bsHash_Init(void);
u32 bsHash_U32(u32 h, u32 v);
u32 bsHash_Bool(u32 h, int b);
u32 bsHash_F32(u32 h, f32 f);
u32 bsHash_Vec3(u32 h, const Vec3* v);
void bsHash_WriteU32BE(u8* dst, u32 v);
u32 bsHash_ReadU32BE(const u8* src);

#endif
