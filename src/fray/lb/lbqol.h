#ifndef FRAY_LB_LBQOL_H
#define FRAY_LB_LBQOL_H

#include "melee/ft/forward.h"
#include <melee/gm/types.h>
#include <sysdolphin/baselib/debug.h>

#define FRAY_ASSERT(cond) HSD_ASSERT(__LINE__, cond)
#define FRAY_ASSERTMSG(cond, msg) HSD_ASSERTMSG(__LINE__, cond, msg)
#define FRAY_ASSERTREPORT(cond, ...)                                          \
    HSD_ASSERTREPORT(__LINE__, cond, __VA_ARGS__)

void Qol_LogInit(void);
CharacterKind Qol_PickRandomTopTier(int worst);
void Qol_SetCompetitivePrefs(void);
void Qol_UnlockAll(void);
extern struct GamePrefs Qol_CompetitiveGamePrefs;
extern GameRules Qol_CompetitiveGameRules;
extern CharacterKind Qol_TierList_PGStats2021[CKind_Playable_Count];

#endif
