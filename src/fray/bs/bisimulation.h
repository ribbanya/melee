#ifndef FRAY_RP_REPLAY_H
#define FRAY_RP_REPLAY_H

#include <melee/gm/forward.h>

#include "fray/bs/bsarchive.h"
#include "fray/bs/bsio.h"
#include "melee/ft/forward.h"
#include "melee/pl/player.h"
#include "Runtime/platform.h"
#include <fray/lb/lbqol.h>
#include <melee/gm/types.h>

#define BISIM_CLASS (FRAY_CLASS_START + 0)
#define BISIM_PLINK (FRAY_PLINK_START + 0)
#define BISIM_PRIORITY 0x80
#define BISIM_USER_DATA_KIND 0
#define BISIM_MAX_SECONDS 30
#define BISIM_MAX_FRAMES (BISIM_MAX_SECONDS * GM_FPS)

#define BISIM_FIGHTER_SIZE 0x4E
#define BISIM_PLAYER_SIZE (0x8 + (BISIM_FIGHTER_SIZE * PL_MAX_SUB_FIGHTERS))
#define BISIM_GLOBAL_SIZE (0x8 + (BISIM_PLAYER_SIZE * GM_MAX_PLAYERS))

typedef u8 Bisim_FighterBuf[BISIM_FIGHTER_SIZE];
typedef u8 Bisim_PlayerBuf[BISIM_PLAYER_SIZE];
typedef u8 Bisim_GlobalBuf[BISIM_GLOBAL_SIZE];

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

/// Snapshot of a ::Fighter
typedef struct {
    bool exists;
    FighterKind ftkind;
    FtMotionId msid;
    float anim_frame;
    GroundOrAir airborne;
    int facing_dir;
    Vec3 pos;
    Vec3 accel;
    Vec3 self_vel;
    Vec3 kb_vel;
    HSD_Pad buttons;
    Vec2 lstick;
    Vec2 cstick;
} Bisim_FighterSnapshot;

/// Snapshot of a ::StaticPlayer
typedef struct {
    bool exists;
    Gm_PKind pkind;
    CharacterKind ckind;
    u8 color;
    s8 stocks;
    u8 team;
    u16 damage;
    Bisim_FighterSnapshot fighters[PL_MAX_SUB_FIGHTERS];
} Bisim_PlayerSnapshot;

/// Global game state snapshot
typedef struct {
    u32 seed;
    u32 curr_frame;
    Bisim_PlayerSnapshot players[GM_MAX_PLAYERS];
} Bisim_GlobalSnapshot;

typedef struct {
    u32 seed;
    u32 hashes[BISIM_MAX_FRAMES];
} Bisim_SeededHashes;

typedef struct {
    Bisim_ArchiveHeader start_header;
    Bisim_GlobalBuf start;
    Bisim_ArchiveHeader history_header;
    Bisim_SeededHashes history;
    Bisim_ArchiveHeader end_header;
    Bisim_GlobalBuf end;
    Bisim_ArchiveHeader tail;
} Bisim_SaveData;

void Bisim_CaptureFighter(Bisim_FighterSnapshot* dst, const Fighter* src);
void Bisim_CapturePlayer(Bisim_PlayerSnapshot* dst, const StaticPlayer* src);
void Bisim_CaptureGlobal(Bisim_GlobalSnapshot* dst);
void Bisim_WriteFighter(BsIO_Cursor* c, const Bisim_FighterSnapshot* v);
void Bisim_WritePlayer(BsIO_Cursor* c, const Bisim_PlayerSnapshot* v);
void Bisim_WriteGlobal(BsIO_Cursor* c, const Bisim_GlobalSnapshot* v);
void Bisim_ReadFighter(BsIO_Cursor* c, Bisim_FighterSnapshot* v);

#endif
