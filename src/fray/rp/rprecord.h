#ifndef FRAY_REPLAY_H
#define FRAY_REPLAY_H

#include <fray/rp/replay.h>
#include <melee/ft/types.h>
#include <melee/gm/types.h>

ReplayFrame* Replay_GetCurrentFrame(int slot);
s8 Replay_GetCpuTrigger(struct CpuFighter* cpu);
void Replay_Mode_OnInit(void);
void Replay_Mode_OnLoad(void);
void Replay_Mode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
