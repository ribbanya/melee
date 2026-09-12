#include "lbqol.h"

#include <Runtime/platform.h>

#include <melee/ft/forward.h>
#include <melee/gm/forward.h>
#include <melee/mn/forward.h>

#include "melee/gr/forward.h"
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_16F1.h>
#include <melee/gm/gmmain_lib.h>
#include <melee/lb/lblanguage.h>
#include <sysdolphin/baselib/debug.h>
#include <sysdolphin/baselib/random.h>

GameRules Qol_CompetitiveGameRules = {
    false,
    52,
    MatchKind_Stock,
    8,
    4,
    0,
    10,
    StageSelectMode_Random,
    8,
    true,
    true,
    0,
    0,
    0,
    8,
    8,
    0,
    0,
    8,
    0,
    -1,
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

StKind Qol_LegalStages[] = {
    St_Kind_Battle,    St_Kind_Shrine,   St_Kind_Story,
    St_Kind_OldPupupu, St_Kind_PStadium, St_Kind_Last,
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

/// @remarks From gmMainLib_8015FA34
void Qol_UnlockAll(void)
{
    gmMainLib_804D3EE0->thing.x186C = U8_MAX;
    gm_80164F18();
    gm_8016468C();
    gm_8017297C();
    gm_801741FC();
}

void Qol_SetCompetitivePrefs(void)
{
    *gmMainLib_GetGamePrefs() = Qol_CompetitiveGamePrefs;
    *gmMainLib_GetGameRules() = Qol_CompetitiveGameRules;
}

CharacterKind Qol_PickRandomTopTier(int max)
{
    FRAY_ASSERT(0 <= max && max < CKind_Playable_Count);
    return Qol_TierList_PGStats2021[HSD_Randi(max)];
}

StKind Qol_PickRandomLegalStage(int max)
{
    FRAY_ASSERT(0 <= max && max < (ssize_t) ARRAY_SIZE(Qol_LegalStages));
    return Qol_LegalStages[HSD_Randi(max)];
}

void* Qol_GetUserDataOrNull(HSD_GObj* gobj)
{
    if (gobj == NULL) {
        return NULL;
    }
    return HSD_GObjGetUserData(gobj);
}
