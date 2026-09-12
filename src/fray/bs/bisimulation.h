#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/forward.h>

#include <fray/lb/lbqol.h>
#include <melee/gm/types.h>
#include <melee/lb/types.h>

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

typedef struct {
} Bisim_FighterSnapshot;

typedef struct {
} Bisim_VsSnapshot;

typedef struct {
    u32 seed;
    BisimState state; ///< ::ReplayState
    u32 curr_frame;
    u32 hashes[BISIM_MAX_FRAMES];
} Replayer;

HSD_GObj* Replay_GetGObj(void);
void Replay_Init(void);

#endif
