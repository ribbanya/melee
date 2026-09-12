///@file bshash.c FNV-1a hashing algorithm

#include "bshash.h"

#include <string.h>

#define FNV_OFFSET 0x811C9DC5U
#define FNV_PRIME 0x01000193U

u32 bsHash_Init(void)
{
    return FNV_OFFSET;
}

u32 bsHash_U32(u32 h, u32 v)
{
    h = (h ^ ((v >> 24) & 0xFF)) * FNV_PRIME;
    h = (h ^ ((v >> 16) & 0xFF)) * FNV_PRIME;
    h = (h ^ ((v >> 8) & 0xFF)) * FNV_PRIME;
    h = (h ^ (v & 0xFF)) * FNV_PRIME;
    return h;
}

u32 bsHash_Bool(u32 h, bool b)
{
    return bsHash_U32(h, b != 0);
}

u32 bsHash_F32Bits(f32 f)
{
#if defined(__MWERKS__)
    return *(u32*) &f;
#else
    u32 u;
    memcpy(&u, &f, 4);
    return u;
#endif
}

u32 bsHash_F32(u32 h, f32 f)
{
    return bsHash_U32(h, bsHash_F32Bits(f));
}

void bsHash_WriteU32BE(u8* dst, u32 v)
{
    dst[0] = (v >> 24) & 0xFF;
    dst[1] = (v >> 16) & 0xFF;
    dst[2] = (v >> 8) & 0xFF;
    dst[3] = v & 0xFF;
}

u32 bsHash_ReadU32BE(const u8* src)
{
    return ((u32) src[0] << 24) | ((u32) src[1] << 16) | ((u32) src[2] << 8) |
           (u32) src[3];
}
