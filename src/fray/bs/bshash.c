///@file bshash.c FNV-1a hashing algorithm

#include "bshash.h"

#include "fray/bs/bsio.h"

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

u32 bsHash_F32(u32 h, f32 f)
{
    return bsHash_U32(h, bsIO_F32Bits(f));
}
