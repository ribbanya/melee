#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/forward.h>

#include <fray/lb/lbqol.h>
#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_CLASS (FRAY_CLASS_START + 0)
#define REPLAY_PLINK (FRAY_PLINK_START + 0)
#define REPLAY_PRIORITY 0x80
#define REPLAY_USER_DATA_KIND 0
#define REPLAY_MAX_SECONDS 30
#define REPLAY_MAX_FRAMES (REPLAY_MAX_SECONDS * GM_FPS)

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
    ReplayVersion_2026_09_11,
    ReplayVersion_Count,
    ReplayVersion_Current = ReplayVersion_Count - 1,
} ReplayVersion;

typedef enum {
    ReplayState_None,
    ReplayState_Recording,
    ReplayState_Verifying,
} ReplayState;

typedef struct {
    u8 enabled_slots : GM_MAX_PLAYERS;
    u32 num_frames;
} ReplayDesc;

typedef struct {
    u32 seed;
    ReplayState state; ///< ::ReplayState
    u32 curr_frame;
    u32 hashes[REPLAY_MAX_FRAMES];
} Replayer;

HSD_GObj* Replay_GetGObj(void);
void Replay_Init(void);
HSD_GObj* Replay_Create(ReplayDesc const* desc);

#endif
