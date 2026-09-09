#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include "melee/gm/forward.h"
#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_MAX_SECONDS 60
#define REPLAY_MAX_FRAMES (GM_FPS * REPLAY_MAX_SECONDS)
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
    u8 stkind;  ///< ::StKind
    u8 mkind;   ///< ::MatchKind
    u8 version; ///< ::ReplayVersion
    u8 reserved;
    u32 seed;
} ReplayMatchDesc;
ASSERT_SIZE(ReplayMatchDesc, 8);

struct CSSData {
    u16 unk_0x0;             ///< 1p port?
    u8 match_type;           ///< ::CSSMatchType
    u8 pending_scene_change; ///< ::CSSPendingSceneChangeKind
    u8* ko_counts;
    VsModeData vs;
};

typedef struct {
    u8 ckind;    ///< ::CharacterKind
    u8 pkind;    ///< ::Gm_PKind
    u8 cpu_kind; ///< ::CpuKind
    u8 color;
    u8 slot;
    u8 spawn_pos;
    ReplayFrame* frames;
} ReplayFighter;

typedef enum {
    ReplayVersion_2026_09_08,
    ReplayVersion_Count,
    ReplayVersion_Current = ReplayVersion_Count - 1,
} ReplayVersion;

typedef enum {
    ReplayState_New,
    ReplayState_Recording,
    ReplayState_Finalized,
} ReplayState;

typedef struct {
    u8 stkind;  ///< ::StKind
    u8 mkind;   ///< ::MatchKind
    u8 version; ///< ::ReplayVersion
    u8 state;   ///< ::ReplayState
    u32 seed;
    u32 num_frames;
    ReplayFighter fighters[GM_MAX_PLAYERS];
} Replay;

HSD_GObj* Replay_GetGObj(void);
void Replay_Init(void);
void Replay_Load(void);

#endif
