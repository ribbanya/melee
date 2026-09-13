#include "bsio.h"

#include <Runtime/platform.h>

#include <string.h>

u32 bsIO_F32Bits(f32 v)
{
#if defined(__MWERKS__)
    return *(u32*) &v;
#else
    u32 u;
    memcpy(&u, &v, 4);
    return u;
#endif
}

void bsIO_Init(BsIO_Cursor* c, u8* buf, u32 cap)
{
    c->buf = buf;
    c->cap = cap;
    bsIO_Reset(c);
}

u32 bsIO_Len(const BsIO_Cursor* c)
{
    return c->pos;
}

u32 bsIO_Remaining(const BsIO_Cursor* c)
{
    if (c->err != BsIO_Ok) {
        return 0;
    }
    return c->cap - c->pos;
}

void bsIO_Reset(BsIO_Cursor* c)
{
    c->pos = 0;
    c->err = BsIO_Ok;
}

static void fail(BsIO_Cursor* c, BsIO_Error err)
{
    if (c->err == BsIO_Ok) {
        c->err = err;
    }
}

void bsIO_WriteU8(BsIO_Cursor* c, u8 v)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + 1 > c->cap) {
        fail(c, BsIO_Overflow);
        return;
    }
    c->buf[c->pos++] = v;
}

void bsIO_WriteU16(BsIO_Cursor* c, u16 v)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + 2 > c->cap) {
        fail(c, BsIO_Overflow);
        return;
    }
    c->buf[c->pos++] = (v >> 8) & 0xFF;
    c->buf[c->pos++] = v & 0xFF;
}

void bsIO_WriteU32(BsIO_Cursor* c, u32 v)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + 4 > c->cap) {
        fail(c, BsIO_Overflow);
        return;
    }
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

void bsIO_WriteVec2(BsIO_Cursor* c, const Vec2* v)
{
    bsIO_WriteF32(c, v->x);
    bsIO_WriteF32(c, v->y);
}

void bsIO_WriteVec3(BsIO_Cursor* c, const Vec3* v)
{
    bsIO_WriteF32(c, v->x);
    bsIO_WriteF32(c, v->y);
    bsIO_WriteF32(c, v->z);
}

void bsIO_WriteBytes(BsIO_Cursor* c, const void* src, u32 n)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + n > c->cap) {
        fail(c, BsIO_Overflow);
        return;
    }
    memcpy(c->buf + c->pos, src, n);
    c->pos += n;
}

u8 bsIO_ReadU8(BsIO_Cursor* c)
{
    if (c->err != BsIO_Ok) {
        return 0;
    }
    if (c->pos + 1 > c->cap) {
        fail(c, BsIO_Underflow);
        return 0;
    }
    return c->buf[c->pos++];
}

u16 bsIO_ReadU16(BsIO_Cursor* c)
{
    u16 hi;
    u16 lo;

    if (c->err != BsIO_Ok) {
        return 0;
    }
    if (c->pos + 2 > c->cap) {
        fail(c, BsIO_Underflow);
        return 0;
    }

    hi = c->buf[c->pos++];
    lo = c->buf[c->pos++];
    return (u16) ((hi << 8) | lo);
}

u32 bsIO_ReadU32(BsIO_Cursor* c)
{
    u32 b0;
    u32 b1;
    u32 b2;
    u32 b3;

    if (c->err != BsIO_Ok) {
        return 0;
    }
    if (c->pos + 4 > c->cap) {
        fail(c, BsIO_Underflow);
        return 0;
    }

    b0 = c->buf[c->pos++];
    b1 = c->buf[c->pos++];
    b2 = c->buf[c->pos++];
    b3 = c->buf[c->pos++];
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

void bsIO_ReadVec2(BsIO_Cursor* c, Vec2* dst)
{
    dst->x = bsIO_ReadF32(c);
    dst->y = bsIO_ReadF32(c);
}

void bsIO_ReadVec3(BsIO_Cursor* c, Vec3* dst)
{
    dst->x = bsIO_ReadF32(c);
    dst->y = bsIO_ReadF32(c);
    dst->z = bsIO_ReadF32(c);
}

void bsIO_ReadBytes(BsIO_Cursor* c, void* dst, u32 n)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + n > c->cap) {
        fail(c, BsIO_Underflow);
        return;
    }
    memcpy(dst, c->buf + c->pos, n);
    c->pos += n;
}

void bsIO_Skip(BsIO_Cursor* c, u32 n)
{
    if (c->err != BsIO_Ok) {
        return;
    }
    if (c->pos + n > c->cap) {
        fail(c, BsIO_Underflow);
        return;
    }
    c->pos += n;
}
