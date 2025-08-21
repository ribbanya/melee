#include "lbshadow.h"

#include "baselib/debug.h"
#include "baselib/shadow.h"
#include "ft/ftlib.h"
#include "lb/types.h"

/// #lbShadow_8000E9F0

/// #lbShadow_8000ED54

static inline HSD_Shadow* lbGetShadow(struct lbShadowContainer* lbshadow)
{
    HSD_ASSERT(98, lbshadow);
    return lbshadow->shadow;
}

void lbShadow_8000EE8C(struct lbShadowContainer* arg0)
{
    HSD_Shadow* shadow;

    if ((shadow = lbGetShadow(arg0)) != NULL) {
        HSD_ShadowRemove(shadow);
    }
}

void lbShadow_8000EEE0(Fighter_GObj* arg0)
{
    bool var_r4;
    bool var_r5;
    bool var_r6;
    bool var_r7;
    bool var_r8;

    if (ftLib_80086960(arg0)) {
        struct lbShadowContainer* lbshadow = ftLib_800872B0(arg0);
        if (lbshadow != NULL) {
            var_r4 = 1;
            var_r5 = 1;
            var_r6 = 1;
            var_r7 = 1;
            var_r8 = 1;
            if (!lbshadow->x0_b0 && !lbshadow->x0_b1) {
                var_r8 = 0;
            }
            if (!var_r8 && !(lbshadow->x0_b2)) {
                var_r7 = 0;
            }
            if (!var_r7 && !lbshadow->x0_b3) {
                var_r6 = 0;
            }
            if (!var_r6 && !lbshadow->x0_b4) {
                var_r5 = 0;
            }
            if (!var_r5 && !lbshadow->x0_b5) {
                var_r4 = 0;
            }
            if (var_r4 == 0 && ftLib_800872BC(arg0)) {
                HSD_ShadowSetActive(lbshadow->shadow, true);
            } else {
                HSD_ShadowSetActive(lbshadow->shadow, false);
            }
        }
    }
}

/// #lbShadow_8000EFEC

/// #lbShadow_8000F214

/// #lbShadow_8000F38C
