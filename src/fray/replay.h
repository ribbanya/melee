#ifndef DOLDECOMP_TESTS_REPLAY_H
#define DOLDECOMP_TESTS_REPLAY_H

#include <melee/gm/types.h>
#include <melee/lb/types.h>
#include <sysdolphin/baselib/controller.h>

typedef struct {
    HSD_Pad buttons;
    S8Vec2 lstick;
    S8Vec2 cstick;
} ReplayFrame;

void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
