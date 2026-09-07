#ifndef DOLDECOMP_TESTS_REPLAY_H
#define DOLDECOMP_TESTS_REPLAY_H

#include <melee/gm/types.h>
#include <sysdolphin/baselib/controller.h>

typedef struct {
    u8 lstick;
    u8 cstick;
    HSD_Pad buttons;
} ReplayFrame;

void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
