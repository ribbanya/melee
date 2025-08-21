#include "lbshadow.h"

#include "baselib/debug.h"
#include "baselib/shadow.h"
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

/// #lbShadow_8000EEE0

/// #lbShadow_8000EFEC

/// #lbShadow_8000F214

/// #lbShadow_8000F38C
