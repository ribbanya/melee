#include "fray/lb/lbqol.h"

#include <Runtime/platform.h>

#include "melee/ft/forward.h"
#include "melee/gm/forward.h"
#include "sysdolphin/baselib/random.h"
#include <melee/gm/gmmain_lib.h>
#include <melee/lb/lblanguage.h>
#include <sysdolphin/baselib/debug.h>

GameRules Qol_CompetitiveGameRules = {
    false, 52,   MatchKind_Stock,
    8,     4,    0,
    10,    0,    8,
    true,  true, 0,
    0,     0,    8,
    8,     0,    0,
    8,     0,    -1,
};

struct GamePrefs Qol_CompetitiveGamePrefs = {
    -1, 0, { false, false, false, false }, 0, true, LANG_US, 0xE70000B0,
};

CharacterKind Qol_TierList_PGStats2021[CKind_Playable_Count] = {
    CKind_Fox,     CKind_Mars,  CKind_Purin,    CKind_Falco,     CKind_Seak,
    CKind_Captain, CKind_Peach, CKind_PopoNana, CKind_Pikachu,   CKind_Yoshi,
    CKind_Samus,   CKind_Luigi, CKind_DrMario,  CKind_Ganon,     CKind_Mario,
    CKind_Donkey,  CKind_CLink, CKind_Link,     CKind_GameWatch, CKind_Mewtwo,
    CKind_Emblem,  CKind_Pichu, CKind_Ness,     CKind_Zelda,     CKind_Kirby,
    CKind_Koopa,
};

static int logPassthrough(UNUSED __file_handle arg0,
                          UNUSED unsigned char* arg1, UNUSED size_t* arg2,
                          UNUSED __idle_proc arg3)
{
    return 0;
}

void reportPassthrough(UNUSED const unsigned char* str, UNUSED size_t len) {}

/// Shut up character-by-character ::OSReport spam.
/// @todo Every ::OSReport is still doubled under Dolphin
void Qol_LogInit(void)
{
    HSD_SetReportCallback(reportPassthrough);

    // Will crash if null.
    stdout->write_proc = logPassthrough;
}

void Qol_SetCompetitivePrefs(void)
{
    *gmMainLib_GetGamePrefs() = Qol_CompetitiveGamePrefs;
    *gmMainLib_GetGameRules() = Qol_CompetitiveGameRules;
}

CharacterKind Qol_PickRandomTopTier(int worst)
{
    FRAY_ASSERT(0 <= worst && worst < CKind_Playable_Count);
    return Qol_TierList_PGStats2021[HSD_Randi(worst)];
}
