#include <Runtime/platform.h>

#include <melee/gm/gmmain_lib.h>
#include <sysdolphin/baselib/debug.h>

GameRules Qol_CompetitiveGameRules = {
    0,
    52,
    MatchKind_Stock, // mode
    8,               // time limit
    4,               // stock count
    0,               // handicap
    10,              // damage ratio
    0,
    0,    // stock time limit
    true, // friendly fire
    true, // pause
    0,    // score display
    0,    // unk_xc
    0,    // xD
    8,    // xE
    8,    // xF
    0,    // unk_x10
    0,    // x11
    8,    // x12
    0,    // x13
    -1,
};
struct gmm_x1CB0 gmMainLib_803D4A60 = {
    0x2, 000000, 0, -1, -1, 0x01010101, 0x00010000, -1, 0,
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
    gmMainLib_GetGamePrefs()->item_freq = -1;
    gmMainLib_GetGameRules()->mode = MatchKind_Stock;
    gmMainLib_GetGameRules()->stock_time_limit = 8;
    gmMainLib_GetGameRules()->stock_count = 4;
    gmMainLib_GetGamePrefs()->stage_mask = 0xE70000B0;
    gmMainLib_GetGameRules()->friendly_fire = true;
}
