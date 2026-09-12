// bsio.c — Cursor-based big-endian serialization for cross-platform state
// hashing.
//
// A BsIO_Cursor points into a caller-provided byte buffer. The same primitives
// read or write depending on direction: bsIO_Put* writes, bsIO_* reads.
// All multi-byte values are big-endian so the byte stream is identical on
// PowerPC and x86.
//
// Buffer sizing is the caller's responsibility. Use a static assert:
//
//     _Static_assert(sizeof(StateHash) <= BIS_STATE_MAX, "");
//
// With that in place, the writer cannot overflow and the reader cannot
// underflow for well-formed blobs.

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

// Float reinterpret. MWCC does not exploit strict-aliasing UB; every other
// compiler gets memcpy, which lowers to a single load at -O1+.
#if defined(__MWERKS__)
static inline u32 f32_bits(f32 f)
{
    return *(u32*) &f;
}
#else
static inline u32 f32_bits(f32 f)
{
    u32 u;
    memcpy(&u, &f, 4);
    return u;
}
#endif

// ============================================================ Cursor

typedef struct {
    u8* buf;
    u32 cap;
    u32 pos;
} BsIO_Cursor;

void bsIO_Init(BsIO_Cursor* c, u8* buf, u32 cap);
u32 bsIO_Len(const BsIO_Cursor* c);
void bsIO_Reset(BsIO_Cursor* c);
u32 bsIO_Remaining(const BsIO_Cursor* c);

// ============================================================ Writers

void bsIO_PutU8(BsIO_Cursor* c, u8 v);
void bsIO_PutU16(BsIO_Cursor* c, u16 v);
void bsIO_PutU32(BsIO_Cursor* c, u32 v);
void bsIO_PutS8(BsIO_Cursor* c, s8 v);
void bsIO_PutS16(BsIO_Cursor* c, s16 v);
void bsIO_PutS32(BsIO_Cursor* c, s32 v);
void bsIO_PutBool(BsIO_Cursor* c, int v);
void bsIO_PutF32(BsIO_Cursor* c, f32 v);
void bsIO_PutBytes(BsIO_Cursor* c, const void* src, u32 n);

// ============================================================ Readers

u8 bsIO_U8(BsIO_Cursor* c);
u16 bsIO_U16(BsIO_Cursor* c);
u32 bsIO_U32(BsIO_Cursor* c);
s8 bsIO_S8(BsIO_Cursor* c);
s16 bsIO_S16(BsIO_Cursor* c);
s32 bsIO_S32(BsIO_Cursor* c);
int bsIO_Bool(BsIO_Cursor* c);
f32 bsIO_F32(BsIO_Cursor* c);
void bsIO_Bytes(BsIO_Cursor* c, void* dst, u32 n);
void bsIO_Skip(BsIO_Cursor* c, u32 n);

// ============================================================ Implementations

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

// ---- Writers ----

void bsIO_PutU8(BsIO_Cursor* c, u8 v)
{
    c->buf[c->pos++] = v;
}

void bsIO_PutU16(BsIO_Cursor* c, u16 v)
{
    c->buf[c->pos++] = (v >> 8) & 0xFF;
    c->buf[c->pos++] = v & 0xFF;
}

void bsIO_PutU32(BsIO_Cursor* c, u32 v)
{
    c->buf[c->pos++] = (v >> 24) & 0xFF;
    c->buf[c->pos++] = (v >> 16) & 0xFF;
    c->buf[c->pos++] = (v >> 8) & 0xFF;
    c->buf[c->pos++] = v & 0xFF;
}

void bsIO_PutS8(BsIO_Cursor* c, s8 v)
{
    bsIO_PutU8(c, (u8) v);
}
void bsIO_PutS16(BsIO_Cursor* c, s16 v)
{
    bsIO_PutU16(c, (u16) v);
}
void bsIO_PutS32(BsIO_Cursor* c, s32 v)
{
    bsIO_PutU32(c, (u32) v);
}

void bsIO_PutBool(BsIO_Cursor* c, int v)
{
    bsIO_PutU8(c, v ? 1 : 0);
}

void bsIO_PutF32(BsIO_Cursor* c, f32 v)
{
    bsIO_PutU32(c, f32_bits(v));
}

void bsIO_PutBytes(BsIO_Cursor* c, const void* src, u32 n)
{
    memcpy(c->buf + c->pos, src, n);
    c->pos += n;
}

// ---- Readers ----

u8 bsIO_U8(BsIO_Cursor* c)
{
    return c->buf[c->pos++];
}

u16 bsIO_U16(BsIO_Cursor* c)
{
    u16 hi = c->buf[c->pos++];
    u16 lo = c->buf[c->pos++];
    return (u16) ((hi << 8) | lo);
}

u32 bsIO_U32(BsIO_Cursor* c)
{
    u32 b0 = c->buf[c->pos++];
    u32 b1 = c->buf[c->pos++];
    u32 b2 = c->buf[c->pos++];
    u32 b3 = c->buf[c->pos++];
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

s8 bsIO_S8(BsIO_Cursor* c)
{
    return (s8) bsIO_U8(c);
}
s16 bsIO_S16(BsIO_Cursor* c)
{
    return (s16) bsIO_U16(c);
}
s32 bsIO_S32(BsIO_Cursor* c)
{
    return (s32) bsIO_U32(c);
}

int bsIO_Bool(BsIO_Cursor* c)
{
    return bsIO_U8(c) != 0;
}

f32 bsIO_F32(BsIO_Cursor* c)
{
    u32 u = bsIO_U32(c);
    f32 f;
    memcpy(&f, &u, 4);
    return f;
}

void bsIO_Bytes(BsIO_Cursor* c, void* dst, u32 n)
{
    memcpy(dst, c->buf + c->pos, n);
    c->pos += n;
}

void bsIO_Skip(BsIO_Cursor* c, u32 n)
{
    c->pos += n;
}
