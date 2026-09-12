#ifndef FRAY_BS_BSHASH_H
#define FRAY_BS_BSHASH_H

#include <Runtime/platform.h>

u32 bsHash_HashU32(u32 h, u32 v);
u32 bsHash_HashBool(u32 h, int b);
u32 bsHash_BitwiseF32(f32 f);
u32 bsHash_HashF32(u32 h, f32 f);
void bsHash_WriteU32BE(u8* dst, u32 v);
u32 bsHash_ReadU32BE(const u8* src);

#endif
