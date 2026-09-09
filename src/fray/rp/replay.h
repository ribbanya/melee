#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/forward.h>
#include <fray/lb/lbqol.h>
#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_CLASS (FRAY_CLASS_START + 0)
#define REPLAY_PLINK (FRAY_PLINK_START + 0)
#define REPLAY_MAX_MINUTES 8
#define REPLAY_MAX_SECONDS (REPLAY_MAX_MINUTES * 60)
#define REPLAY_MAX_FRAMES (REPLAY_MAX_SECONDS * GM_FPS)
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
    u8 trigger;
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

typedef struct {
    u32 seed;
    VsModeData vs;
} ReplaySetupData;

typedef enum {
    ReplayVersion_2026_09_08,
    ReplayVersion_Count,
    ReplayVersion_Current = ReplayVersion_Count - 1,
} ReplayVersion;

typedef enum {
    ReplayState_New,
    ReplayState_Recording,
    ReplayState_Finalized,
    ReplayState_Playing,
} ReplayState;

typedef struct {
    u8 enabled_slots : GM_MAX_PLAYERS;
    u32 num_frames;
} ReplayDesc;

typedef struct {
    u32 seed;
    ReplayState state; ///< ::ReplayState
    u32 num_frames;
    u32 cur_frame;
    ReplayFrame* frames[GM_MAX_PLAYERS];
} Replayer;

HSD_GObj* Replay_GetGObj(void);
void Replay_Init(void);
void Replay_Create(ReplayDesc const* desc);

#endif
