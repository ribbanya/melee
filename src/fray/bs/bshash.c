///@file bshash.c FNV-1a hashing algorithm

#include "bshash.h"

#include "bsio.h"

#define FNV_OFFSET 0x811C9DC5U
#define FNV_PRIME 0x01000193U

u32 bsHash_Init(void)
{
    return FNV_OFFSET;
}

u32 bsHash_U8(u32 h, u8 v)
{
    return (h ^ v) * FNV_PRIME;
}

u32 bsHash_Bytes(u32 h, const u8* v, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        h = bsHash_U8(h, v[i]);
    }
    return h;
}

u32 bsHash_Cursor(u32 h, const BsIO_Cursor* v)
{
    return bsHash_Bytes(h, v->buf, v->pos);
}

u32 bsHash_U16(u32 h, u16 v)
{
    h = bsHash_U8(h, (v >> 8) & 0xFF);
    h = bsHash_U8(h, v & 0xFF);
    return h;
}

u32 bsHash_U32(u32 h, u32 v)
{
    h = bsHash_U8(h, (v >> 24) & 0xFF);
    h = bsHash_U8(h, (v >> 16) & 0xFF);
    h = bsHash_U8(h, (v >> 8) & 0xFF);
    h = bsHash_U8(h, v & 0xFF);
    return h;
}

u32 bsHash_Bool(u32 h, bool v)
{
    return bsHash_U32(h, v != 0);
}

u32 bsHash_F32(u32 h, f32 v)
{
    return bsHash_U32(h, bsIO_F32Bits(v));
}

u32 bsHash_Vec3(u32 h, const Vec3* v)
{
    h = bsHash_F32(h, v->x);
    h = bsHash_F32(h, v->y);
    h = bsHash_F32(h, v->z);
    return h;
}
