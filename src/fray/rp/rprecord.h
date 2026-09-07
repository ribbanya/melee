#ifndef FRAY_REPLAY_H
#define FRAY_REPLAY_H

#include <melee/gm/types.h>
#include <melee/lb/types.h>

#define REPLAY_MAX_LENGTH (GM_FPS * 60)

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
} ReplayFrame;

typedef struct {
    u8 pkind; ///< ::Gm_PKind
    u8 ckind; ///< ::CharacterKind
    u8 color;
    u8 slot;
    u8 spawn_pos;
    ReplayFrame frames[REPLAY_MAX_LENGTH];
} ReplayFighter;

void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
