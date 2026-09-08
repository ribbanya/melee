#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_MAX_SECONDS 60
#define REPLAY_MAX_FRAMES (GM_FPS * REPLAY_MAX_SECONDS)
#define REPLAY_VERSION 0
#define FRAY_GOBJ_CLASS_SHIFT 5
#define REPLAY_GOBJ_CLASS (1 << FRAY_GOBJ_CLASS_SHIFT)

typedef struct {
    u8 a : 1;
    u8 b : 1;
    u8 x : 1;
    u8 y : 1;
    u8 l : 1;
    u8 r : 1;
    u8 z : 1;
    u8 dpad_up : 1;
    S8Vec2 lstick;
    S8Vec2 cstick;
} ReplayInputs;

typedef struct {
    u8 trigger;
    u8 facing_left : 1;
    u8 airborne : 1;
    u8 ecb_locked : 1;
    u8 hit_this_frame : 1;
    u8 reserved;
} ReplayOutputs;

typedef struct {
    ReplayInputs in;
    ReplayOutputs out;
} ReplayFrame;
ASSERT_SIZE(ReplayFrame, 8);

typedef struct {
    u8 ckind;    ///< ::CharacterKind
    u8 pkind;    ///< ::Gm_PKind
    u8 cpu_kind; ///< ::CpuKind
    u8 color;
    u8 slot;
    u8 spawn_pos;
    u8 reserved[2];
} ReplayFighterDesc;
ASSERT_SIZE(ReplayFighterDesc, 8);

typedef struct {
    u8 stkind; ///< ::StKind
    u8 mkind;  ///< ::MatchKind
    u8 version;
    u8 reserved;
    u32 seed;
} ReplayMatchDesc;
ASSERT_SIZE(ReplayMatchDesc, 8);

typedef struct {
    ReplayFighterDesc desc;
    ReplayFrame* frames;
} ReplayFighter;

typedef struct {
    ReplayMatchDesc desc;
    u32 num_frames;
    u32 num_fighters;
    ReplayFighter** fighters;
} Replay;

#endif
