#include <Runtime/platform.h>

#include <melee/gm/gmmain_lib.h>
#include <melee/lb/lblanguage.h>
#include <sysdolphin/baselib/debug.h>

GameRules Qol_CompetitiveGameRules = {
    0,    52, MatchKind_Stock,
    8,    4,  0,
    0,    0,  true,
    true, 0,  0,
    0,    8,  8,
    0,    0,  8,
    0,    -1, 0,
};

struct gmm_x1CB0 Qol_CompetitiveGamePrefs = {
    -1, 0, { false, false, false, false }, 0, true, LANG_US, 0xE70000B0,
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
