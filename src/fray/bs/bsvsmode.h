#ifndef FRAY_REPLAY_H
#define FRAY_REPLAY_H

#include <fray/bs/bisimulation.h>
#include <melee/gm/types.h>

Bisim_SaveData* bsVsMode_GetSaveData(void);
void Replay_Mode_OnInit(void);
void bsVsMode_OnLoad(void);
void bsVsMode_OnUnload(void);
extern GameModeState Replay_RecordStates[];

#endif
