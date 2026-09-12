#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/forward.h>

#include "melee/ft/forward.h"
#include "melee/pl/player.h"
#include "Runtime/platform.h"
#include <fray/lb/lbqol.h>
#include <melee/gm/types.h>

#define BISIM_CLASS (FRAY_CLASS_START + 0)
#define BISIM_PLINK (FRAY_PLINK_START + 0)
#define BISIM_PRIORITY 0x80
#define BISIM_USER_DATA_KIND 0
#define BISIM_MAX_SECONDS 30
#define BISIM_MAX_FRAMES (BISIM_MAX_SECONDS * GM_FPS)

typedef struct {
    u32 seed;
    VsModeData vs;
} ReplaySetupData;

typedef enum {
    BisimVersion_2026_09_11,
    BisimVersion_Count,
    BisimVersion_Current = BisimVersion_Count - 1,
} BisimVersion;

typedef enum {
    BisimState_None,
    BisimState_Recording,
    BisimState_Verifying,
} BisimState;

/// Snapshot of a ::Fighter
typedef struct {
    FtMotionId msid;
    float anim_frame;
    GroundOrAir airborne;
    f32 facing_dir;
    Vec3 pos;
    Vec3 accel;
    Vec3 vel;
    Vec3 kb_vel;
} Bisim_FighterSnapshot;
ASSERT_SIZE(Bisim_FighterSnapshot, 64);

/// Snapshot of a ::StaticPlayer
typedef struct {
    Gm_PKind pkind;
    u8 color;
    u8 port;
    Bisim_FighterSnapshot sub_fighters[PL_MAX_SUB_FIGHTERS];
} Bisim_PlayerSnapshot;

/// Global game state snapshot
typedef struct {
    u32 seed;
    u32 curr_frame;
    Bisim_PlayerSnapshot players[GM_MAX_PLAYERS];
} Bisim_GlobalSnapshot;

typedef struct {
    u32 seed;
    BisimState state; ///< ::ReplayState
    u32 curr_frame;
    u32 hashes[BISIM_MAX_FRAMES];
} Replayer;

HSD_GObj* Replay_GetGObj(void);
void Replay_Init(void);

#endif
