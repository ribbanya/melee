#ifndef FRAY_BS_BSIO_H
#define FRAY_BS_BSIO_H

#include <Runtime/platform.h>

#include <dolphin/mtx.h>

typedef struct {
    u8* buf;
    u32 cap;
    u32 pos;
} BsIO_Cursor;

void bsIO_Init(BsIO_Cursor* c, u8* buf, u32 cap);
u32 bsIO_Len(const BsIO_Cursor* c);
void bsIO_Reset(BsIO_Cursor* c);
u32 bsIO_Remaining(const BsIO_Cursor* c);

u32 bsIO_F32Bits(f32 f);
void bsIO_Skip(BsIO_Cursor* c, u32 n);

u8 bsIO_ReadU8(BsIO_Cursor* c);
u16 bsIO_ReadU16(BsIO_Cursor* c);
u32 bsIO_ReadU32(BsIO_Cursor* c);
s8 bsIO_ReadS8(BsIO_Cursor* c);
s16 bsIO_ReadS16(BsIO_Cursor* c);
s32 bsIO_ReadS32(BsIO_Cursor* c);
int bsIO_ReadBool(BsIO_Cursor* c);
f32 bsIO_ReadF32(BsIO_Cursor* c);
void bsIO_ReadVec3(BsIO_Cursor* c, Vec3* dst);
void bsIO_ReadBytes(BsIO_Cursor* c, void* dst, u32 n);

void bsIO_WriteU8(BsIO_Cursor* c, u8 v);
void bsIO_WriteU16(BsIO_Cursor* c, u16 v);
void bsIO_WriteU32(BsIO_Cursor* c, u32 v);
void bsIO_WriteS8(BsIO_Cursor* c, s8 v);
void bsIO_WriteS16(BsIO_Cursor* c, s16 v);
void bsIO_WriteS32(BsIO_Cursor* c, s32 v);
void bsIO_WriteBool(BsIO_Cursor* c, int v);
void bsIO_WriteF32(BsIO_Cursor* c, f32 v);
void bsIO_WriteVec3(BsIO_Cursor* c, const Vec3* v);
void bsIO_WriteBytes(BsIO_Cursor* c, const void* src, u32 n);

#endif
