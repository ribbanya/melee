#include <melee/gm/types.h>

enum {
    state_record_vs,
};

GameModeState gm_Mode_DebugVs_States[] = {
    {
        state_record_vs,
        lbDvdPreload_2,
        0,
        onEnterDebugVs,
        NULL,
        {
            GS_VS,
            &gmVsMelee_StartData,
            &gmVsMelee_VsExitInfo,
        },
    },
    {
        state_debug_results,
        lbDvdPreload_2,
        0,
        onEnterResults,
        NULL,
        {
            GS_RESULTS,
            &gmVsMelee_ResultsEnterData,
            NULL,
        },
    },
    { GM_GAMEMODESTATE_TERMINATE },
};
