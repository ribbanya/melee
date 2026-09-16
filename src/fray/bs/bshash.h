#ifndef FRAY_BS_BSHASH_H
#define FRAY_BS_BSHASH_H

#include <Runtime/platform.h>

#include <dolphin/mtx.h>
#include <fray/bs/bsio.h>

u32 bsHash_Init(void);
u32 bsHash_U8(u32 h, u8 v);
u32 bsHash_Bytes(u32 h, const u8* v, size_t n);
u32 bsHash_Cursor(u32 h, const BsIO_Cursor* v);
u32 bsHash_U16(u32 h, u16 v);
u32 bsHash_U32(u32 h, u32 v);
u32 bsHash_Bool(u32 h, bool v);
u32 bsHash_F32(u32 h, f32 v);
u32 bsHash_Vec3(u32 h, const Vec3* v);

#endif
