#include "gm_1A3F.h"

#include "gm_1A45.h"
#include "gmmain_lib.h"

#include "db/db.h"
#include "gm/gm_1A36.h"
#include "gm/gmscdata.h"
#include "lb/lbaudio_ax.h"
#include "lb/lbdvd.h"
#include "lb/lbheap.h"
#include "lb/lbmthp.h"
#include "lb/lbsnap.h"
#include "lb/types.h"
#include "ty/toy.h"

#include <dolphin/vi.h>
#include <baselib/controller.h>
#include <baselib/devcom.h>
#include <baselib/sislib.h>
#include <baselib/video.h>
#include <melee/gm/types.h>
#include <melee/lb/lb_00B0.h>
#include <melee/lb/lbcardgame.h>
#include <melee/lb/lbcardnew.h>
#include <melee/ty/tydisplay.h>

typedef struct {
    u8 curr_mode;    ///< GameModeKind
    u8 pending_mode; ///< GameModeKind
    u8 prev_mode;    ///< GameModeKind
    u8 curr_scene_idx;
    u8 prev_scene_idx;
    u8 pending_scene_idx;
} GameRouting;
ASSERT_SIZE(GameRouting, 0x6);

typedef struct {
    GameRouting routing;
    GameRouting backup;
    u8 pending;
    u8 xD;
    u8 xE;
    u8 xF;
    u8 (*game_mode_override)(void);
} GameModeStateMachine;
ASSERT_SIZE(GameModeStateMachine, 0x14);

/* 1A3F48 */ static void gm_801A3F48(GameModeState*);
/* 479D30 */ static GameModeStateMachine state_machine;

void gm_801A3F48(GameModeState* state)
{
    PreloadedGameModeState* preloaded_state;

    lbDvd_80018CF4(state->preload);
    switch (state->info.scene_id) {
    case GS_STAFFROLL:
    case GS_RESULTS:
        HSD_SisLib_803A6048(0xC000);
        break;
    case GS_CSS:
        HSD_SisLib_803A6048(0x2400);
        break;
    default:
        HSD_SisLib_803A6048(0x4800);
        break;
    }
    preloaded_state = lbDvd_GetPreloadCacheScene();
    if (lbHeap_80015BB8(2) == 0) {
        preloaded_state->is_heap_persistent[0] = true;
    }
    if (lbHeap_80015BB8(3) == 0) {
        preloaded_state->is_heap_persistent[1] = true;
    }
    lbDvd_80018254();
    lb_8001C5A4();
    lb_8001D1F4();
    lbSnap_8001E27C();
    Toy_803127D4();
    tyDisplay_8031C8B8();
}

static inline u8 firstState(GameModeState* state, u8 sentinel)
{
    for (; state->idx != (u8) -1; state++) {
        do {
            if (state->idx == sentinel) {
                break;
            }
        } while (0);
        return state->idx;
    }
    return 0;
}

static inline u8 nextState(GameModeState* states)
{
    GameModeState* it = states;
    u8 current = state_machine.routing.curr_scene_idx;
    int i;
    u8 next_scene;
    GameModeState* cur = states;

    for (i = 0; (next_scene = it->idx) != (u8) -1; i++) {
        if (cur->idx > current) {
            return states[i].idx;
        }
        cur++;
        it++;
    }

    return firstState(states, next_scene);
}

static inline GameModeState* findState(GameModeState* state)
{
    int i, j;
    for (i = state_machine.routing.curr_scene_idx; i < (u8) -1; i++) {
        for (j = 0; state[j].idx != (u8) -1; j++) {
            if (i == state[j].idx) {
                return &state[j];
            }
        }
    }
    return NULL;
}

void gm_801A4014(GameMode* mode)
{
    GameSceneHandler* handler;
    GameModeState* scene;
    GameModeStateMachine* gm;
    struct GameSceneInfo* info;
    u32 dead;
    PAD_STACK(2 * 4);

    gm = &state_machine;

    scene = findState(mode->states);

    gm->routing.curr_scene_idx = scene->idx;

    gm_801A3F48(scene);
    if (scene->on_enter != NULL) {
        scene->on_enter(scene);
    }
    info = &scene->info;
    handler = (GameSceneHandler*) ((uintptr_t) gm_FindGameSceneHandler(
                                       info->scene_id) |
                                   (dead = 0));
    gm_801A4BD4();
    gm_801A4B88(info);
    if (handler->on_load != NULL) {
        handler->on_load(info->load_data);
    }
    gm_801A4D34(handler->on_frame, info);
    if (!gmMainLib_8046B0F0.resetting && handler->on_leave != NULL) {
        handler->on_leave(info->exit_data);
    }
    if (!gmMainLib_8046B0F0.resetting) {
        if (scene->on_exit != NULL) {
            scene->on_exit(scene);
        }

        state_machine.routing.prev_scene_idx = gm->routing.curr_scene_idx;

        if (gm->routing.pending_scene_idx) {
            gm->routing.curr_scene_idx = gm->routing.pending_scene_idx - 1;
            gm->routing.pending_scene_idx = 0;
        } else {
            gm->routing.curr_scene_idx = nextState(mode->states);
        }
    }
    lb_8001CDB4();
    lb_8001B760(0xB);
    lbMthp_8001F800();
    if (gmMainLib_8046B0F0.resetting) {
        lbAudioAx_80027DBC();
        HSD_PadReset();
        while (lb_8001B6F8() == 0xB)
            ;
        if (DVDCheckDisk() == 0) {
            OSResetSystem(1, 0, 0);
        }
        lbMthp_8001F800();
        while (HSD_DevComIsBusy(1))
            ;
        gmMainLib_8015FBA4();
        gm_GetAllGameModes();
        memzero(&state_machine, sizeof(state_machine));
        gm_801A3EF4();
        gmMainLib_8046B0F0.skip_intro = true;
        gm_ChangeGameModeAfterCurrentScene(GM_BOOT);
        HSD_VISetBlack(0);
    }
}

void* gm_GetGameSceneLoadData(GameModeState* scene)
{
    return scene->info.load_data;
}

void* gm_GetGameSceneLeaveData(GameModeState* scene)
{
    return scene->info.exit_data;
}

void gm_SetSceneIndex(u8 arg0)
{
    state_machine.routing.curr_scene_idx = arg0;
    state_machine.routing.prev_scene_idx = arg0;
}

/// @note Actually sets the pending scene to the scene following the input
void gm_SetPendingSceneIndex(u8 pending_scene)
{
    state_machine.routing.pending_scene_idx = pending_scene + 1;
}

u8 gm_GetPreviousSceneIndex(void)
{
    return state_machine.routing.prev_scene_idx;
}

u8 gm_GetCurrentSceneIndex(void)
{
    return state_machine.routing.curr_scene_idx;
}

void gm_SetNewGameModePending(void)
{
    state_machine.pending = 1;
}

void gm_SetPendingGameMode(s8 pending_mode)
{
    state_machine.routing.pending_mode = pending_mode;
}

void gm_ChangeGameModeAfterCurrentScene(int pending_mode)
{
    state_machine.routing.pending_mode = pending_mode;
    state_machine.pending = 1;
}

u8 gm_GetCurrentGameMode(void)
{
    return state_machine.routing.curr_mode;
}

u8 gm_GetPreviousGameMode(void)
{
    return state_machine.routing.prev_mode;
}

void gm_SetGameModeOverride(u8 (*mode)(void))
{
    state_machine.game_mode_override = mode;
}

bool gm_Is1PMode(u8 mode)
{
    switch (mode) {
    case GM_CLASSIC:
    case GM_ADVENTURE:
    case GM_ALLSTAR:
    case GM_TARGET_TEST:
    case GM_TRAINING:
    case GM_HOME_RUN_CONTEST:
    case GM_10MAN_VS:
    case GM_100MAN_VS:
    case GM_3MIN_VS:
    case GM_15MIN_VS:
    case GM_ENDLESS_VS:
    case GM_CRUEL_VS:
    case GM_EVENT:
        return true;
    }
    return false;
}

static inline GameMode* findMode(u8 idx)
{
    GameMode* cur;
    for (cur = gm_GetAllGameModes(); cur->kind != GM_COUNT; cur++) {
        if (cur->kind == idx) {
            return cur;
        }
    }
    return NULL;
}

u8 gm_RunGameMode(u8 mode_kind)
{
    u8 temp_r3;
    GameMode* mode;
    GameMode* var_r3_2;
    GameModeStateMachine* gamestate = &state_machine;
    PAD_STACK(2 * 4);

    mode = findMode(mode_kind);

    state_machine.pending = 0;
    state_machine.routing.curr_scene_idx = 0;
    state_machine.routing.prev_scene_idx = 0;
    state_machine.routing.pending_scene_idx = 0;
    lbDvd_80018F58(mode->preload);
    if (mode->on_load != NULL) {
        mode->on_load();
    }
    while (!gamestate->pending) {
        if (state_machine.game_mode_override != NULL &&
            (temp_r3 = state_machine.game_mode_override(),
             temp_r3 != GM_COUNT))
        {
            state_machine.backup = state_machine.routing;
            gamestate->pending = 0;
            gamestate->routing.curr_scene_idx = 0;
            gamestate->routing.prev_scene_idx = 0;
            gamestate->routing.pending_scene_idx = 0;

            var_r3_2 = findMode(temp_r3);

            gm_801A4014(var_r3_2);
            if (!gmMainLib_8046B0F0.resetting) {
                state_machine.routing = state_machine.backup;
            }
        } else {
            gm_801A4014(mode);
        }
    }
    if (!gmMainLib_8046B0F0.resetting && mode->on_unload != NULL) {
        mode->on_unload();
    }
    return state_machine.routing.pending_mode;
}

/// UnclePunch: Scene_Main
void gm_801A4510(void)
{
    u32 unused;
    GameMode* modes;
    GameModeStateMachine* gamestate = &state_machine;
    int i;

    gm_GetAllGameModes();
    memzero(&state_machine, sizeof(GameModeStateMachine));
    modes = gm_GetAllGameModes();
    for (i = 0; modes[i].kind != GM_COUNT; i++) {
        if (modes[i].on_init != NULL) {
            modes[i].on_init();
        }
    }
    if (VIGetDTVStatus() != 0 &&
        (db_gameLaunchButtonState & HSD_PAD_B || OSGetProgressiveMode() == 1))
    {
        state_machine.routing.curr_mode = GM_PROGRESSIVE_SCAN;
    } else {
        state_machine.routing.curr_mode = GM_BOOT;
    }
    state_machine.routing.prev_mode = GM_COUNT;

    while (true) {
        u8 next_mode = gm_RunGameMode(state_machine.routing.curr_mode);
        if (gmMainLib_8046B0F0.resetting) {
            gmMainLib_8046B0F0.resetting = false;
        }
        gamestate->routing.prev_mode = gamestate->routing.curr_mode;
        gamestate->routing.curr_mode = next_mode;
    }
}
