#ifndef GALE01_2599EC
#define GALE01_2599EC

#include <placeholder.h>

#include "platform.h"

#include "baselib/jobj.h"

struct _m2c_stack_mnStageSel_8025A998 {
    /* 0x000 */ char pad_0[0xCC];
    /* 0x0CC */ s32 spCC; /* inferred */
    /* 0x0D0 */ s32 spD0; /* inferred */
    /* 0x0D4 */ s32 spD4; /* inferred */
    /* 0x0D8 */ char pad_D8[4];
    /* 0x0DC */ HSD_JObj* spDC;    /* inferred */
    /* 0x0E0 */ char pad_E0[0x3C]; /* maybe part of spDC[0x10]? */
    /* 0x11C */ HSD_JObj* sp11C;   /* inferred */
    /* 0x120 */ char pad_120[0x40];
}; /* size = 0x160 */

struct _struct_mnStageSel_803F06D0_0x1C {
    /* 0x00 */ void* unk_0; /* inferred */
    /* 0x04 */ char pad_4[4];
    /* 0x08 */ u8 unk_8; /* inferred */
    /* 0x09 */ u8 unk_9; /* inferred */
    /* 0x0A */ char pad_A[1];
    /* 0x0B */ u8 unk_B;         /* inferred */
    /* 0x0C */ char pad_C[0x10]; /* maybe part of unk_B[0x11]? */
}; /* size = 0x1C */

#endif
