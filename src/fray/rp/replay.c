#include "replay.h"

#include <Runtime/platform.h>

#include <melee/gm/forward.h>
#include <melee/pl/forward.h>

#include <abort_exit.h> // IWYU pragma: keep

#include "melee/lb/forward.h"
#include "rpcard.h"
#include "rpdisplay.h"
#include <dolphin/types.h>
#include <fray/lb/lbqol.h>
#include <melee/ft/types.h>
#include <melee/gm/gm_1601.h>
#include <melee/gm/gm_16AE.h>
#include <melee/gm/gm_1A3F.h>
#include <melee/gm/gm_1B03.h>
#include <melee/gm/gmvsmelee.h>
#include <melee/gm/types.h>
#include <melee/lb/lb_00B0.h>
#include <melee/pl/player.h>
#include <sysdolphin/baselib/controller.h>
#include <sysdolphin/baselib/random.h>

static

    static ReplayFighterConfig const fighter_init[] = {
        { CKIND_FOX, true, 0, 0, 2 },
        { CKIND_FOX, true, 2, 3, 3 },
    };
