#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_MAX_SECONDS 60
#define REPLAY_MAX_FRAMES (GM_FPS * 60)
#define REPLAY_NUM_FIGHTERS 2
#define REPLAY_VERSION 0
#define REPLAY_SEED 0xDEEDBEEF

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
    u8 trigger;
    u8 facing_left : 1;
    u8 airborne : 1;
    u8 ecb_locked : 1;
    u8 hit_this_frame : 1;
    u8 reserved;
} ReplayFrame;

typedef struct {
    u8 ckind : 6; ///< ::CharacterKind
    u8 is_cpu : 1;
    u8 color : 3;
    u8 slot : 2;
    u8 spawn_pos : 2;
} ReplayFighterConfig;

typedef struct {
    u8 replay_version;
    u8 num_fighters;
    u32 seed;
} ReplayInit;

typedef struct {
    u8 num_fighters;
    u32 num_frames;
    ReplayFrame** frames;
} Replay;

#endif
