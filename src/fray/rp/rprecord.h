#ifndef FRAY_REPLAY_H
#define FRAY_REPLAY_H

#include <melee/ft/types.h>
#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_MAX_SECONDS 60
#define REPLAY_MAX_FRAMES (GM_FPS * 60)

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
} ReplayFrame;

typedef struct {
    u8 ckind : 6; ///< ::CharacterKind
    u8 is_cpu : 1;
    u8 color : 3;
    u8 slot : 2;
    u8 spawn_pos : 2;
} ReplayFighterInit;

typedef struct {
    ReplayFighterInit init;
    ReplayFrame frames[REPLAY_MAX_FRAMES];
} ReplayFighter;

ReplayFrame* Replay_GetCurrentFrame(int slot);
s8 Replay_GetCpuTrigger(struct CpuFighter* cpu);
void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
