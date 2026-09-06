#ifndef DOLDECOMP_TESTS_REPLAY_H
#define DOLDECOMP_TESTS_REPLAY_H

#include <melee/gm/types.h>

void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
