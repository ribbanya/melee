#include "bsio.h"

#include <Runtime/platform.h>

#include <string.h>

u32 bsIO_F32Bits(f32 f)
{
#if defined(__MWERKS__)
    return *(u32*) &f;
#else
    u32 u;
    memcpy(&u, &f, 4);
    return u;
#endif
}

void bsIO_Init(BsIO_Cursor* c, u8* buf, u32 cap)
{
    c->buf = buf;
    c->cap = cap;
    c->pos = 0;
}

u32 bsIO_Len(const BsIO_Cursor* c)
{
    return c->pos;
}

void bsIO_Reset(BsIO_Cursor* c)
{
    c->pos = 0;
}

u32 bsIO_Remaining(const BsIO_Cursor* c)
{
    return c->cap - c->pos;
}

void bsIO_WriteU8(BsIO_Cursor* c, u8 v)
{
    c->buf[c->pos++] = v;
}

void bsIO_WriteU16(BsIO_Cursor* c, u16 v)
{
    c->buf[c->pos++] = (v >> 8) & 0xFF;
    c->buf[c->pos++] = v & 0xFF;
}

void bsIO_WriteU32(BsIO_Cursor* c, u32 v)
{
    c->buf[c->pos++] = (v >> 24) & 0xFF;
    c->buf[c->pos++] = (v >> 16) & 0xFF;
    c->buf[c->pos++] = (v >> 8) & 0xFF;
    c->buf[c->pos++] = v & 0xFF;
}

void bsIO_WriteS8(BsIO_Cursor* c, s8 v)
{
    bsIO_WriteU8(c, (u8) v);
}
void bsIO_WriteS16(BsIO_Cursor* c, s16 v)
{
    bsIO_WriteU16(c, (u16) v);
}
void bsIO_WriteS32(BsIO_Cursor* c, s32 v)
{
    bsIO_WriteU32(c, (u32) v);
}

void bsIO_WriteBool(BsIO_Cursor* c, bool v)
{
    bsIO_WriteU8(c, v != 0);
}

void bsIO_WriteF32(BsIO_Cursor* c, f32 v)
{
    bsIO_WriteU32(c, bsIO_F32Bits(v));
}

void bsIO_WriteVec3(BsIO_Cursor* c, const Vec3* v)
{
    bsIO_WriteF32(c, v->x);
    bsIO_WriteF32(c, v->y);
    bsIO_WriteF32(c, v->z);
}

void bsIO_WriteBytes(BsIO_Cursor* c, const void* src, u32 n)
{
    memcpy(c->buf + c->pos, src, n);
    c->pos += n;
}

u8 bsIO_ReadU8(BsIO_Cursor* c)
{
    return c->buf[c->pos++];
}

u16 bsIO_ReadU16(BsIO_Cursor* c)
{
    u16 hi = c->buf[c->pos++];
    u16 lo = c->buf[c->pos++];
    return (u16) ((hi << 8) | lo);
}

u32 bsIO_ReadU32(BsIO_Cursor* c)
{
    u32 b0 = c->buf[c->pos++];
    u32 b1 = c->buf[c->pos++];
    u32 b2 = c->buf[c->pos++];
    u32 b3 = c->buf[c->pos++];
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

s8 bsIO_ReadS8(BsIO_Cursor* c)
{
    return (s8) bsIO_ReadU8(c);
}

s16 bsIO_ReadS16(BsIO_Cursor* c)
{
    return (s16) bsIO_ReadU16(c);
}

s32 bsIO_ReadS32(BsIO_Cursor* c)
{
    return (s32) bsIO_ReadU32(c);
}

int bsIO_ReadBool(BsIO_Cursor* c)
{
    return bsIO_ReadU8(c) != 0;
}

f32 bsIO_ReadF32(BsIO_Cursor* c)
{
    return bsIO_F32Bits(bsIO_ReadU32(c));
}

void bsIO_ReadVec3(BsIO_Cursor* c, Vec3* dst)
{
    dst->x = bsIO_ReadF32(c);
    dst->y = bsIO_ReadF32(c);
    dst->z = bsIO_ReadF32(c);
}

void bsIO_ReadBytes(BsIO_Cursor* c, void* dst, u32 n)
{
    memcpy(dst, c->buf + c->pos, n);
    c->pos += n;
}

void bsIO_MemZero(void* dst, u32 n)
{
    memset(dst, 0, (size_t) n);
}

void bsIO_Skip(BsIO_Cursor* c, u32 n)
{
    c->pos += n;
}
