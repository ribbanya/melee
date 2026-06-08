#include "ftkirby.h"
#include "ftkirbyspeciallw.h"
#include "types.h"

#include <placeholder.h>

#include "baselib/debug.h"
#include "ef/eflib.h"
#include "ef/efsync.h"
#include "ft/fighter.h"

#include "ft/forward.h"

#include "ft/ft_081B.h"
#include "ft/ft_0892.h"
#include "ft/ftanim.h"
#include "ft/inlines.h"
#include "ft/types.h"
#include "ftCommon/ftCo_Fall.h"
#include "ftCommon/ftCo_Landing.h"

#include "ftKirby/forward.h"

#include "ftKirby/inlines.h"
#include "it/items/itkirbyhammer.h"
#include "lb/lb_00B0.h"

#include <common_structs.h>
#include <stddef.h>
#include <trigf.h>
#include <baselib/gobj.h>
#include <baselib/random.h>
#include <MSL/math.h>

static inline void fn_800F53AC_SpawnEffect(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    s32 ef_id;
    if (fp->ground_or_air == GA_Air) {
        ef_id = 0x497;
    } else {
        ef_id = 0x496;
    }
    efSync_Spawn(ef_id, gobj, fp->parts[FtPart_HipN].joint, &fp->facing_dir);
    fp->x2219_b0 = true;
    fp->pre_hitlag_cb = efLib_PauseAll;
    fp->post_hitlag_cb = efLib_ResumeAll;
}

static inline void fn_800F53AC_CleanupItem(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    Item_GObj* item = fp->fv.kb.hat.x0;
    if (item != NULL) {
        it_802ADC34(item);
        fp->fv.kb.hat.x0 = NULL;
    }
}

void fn_800F53AC(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    Vec3 pos;

    if (fp->cmd_vars[0] == 1) {
        f32 dir;
        lb_8000B1CC(fp->parts[FtPart_R3rdNa].joint, NULL, &pos);
        dir = fp->facing_dir;
        fp->fv.kb.hat.x0 =
            it_802ADC54(gobj, &pos, FtPart_R3rdNa, fp->ground_or_air, dir);
        fp->x1984_heldItemSpec = fp->fv.kb.hat.x0;
        fp->cmd_vars[0] = 0;
        if (fp->fv.kb.hat.x0 != NULL) {
            if (fp->death2_cb != NULL && fp->death2_cb != ftKb_Init_800EE74C) {
                OSReport("Other Dead_Proc Existence", fp->death2_cb,
                         fp->fv.kb.hat.x0);
                HSD_ASSERT(102, 0);
            }
            fp->death2_cb = ftKb_Init_800EE74C;
            fp->take_dmg_cb = ftKb_Init_800EE7B8;
            fn_800F53AC_SpawnEffect(gobj);
        }
    }
    if (fp->cmd_vars[0] == 2) {
        fn_800F53AC_CleanupItem(gobj);
        fp->cmd_vars[0] = 0;
    }
}

void ftKb_SpecialAirLw_800F5524(HSD_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    if (fp->fv.kb.hat.x0 != NULL) {
        it_802ADC34(fp->fv.kb.hat.x0);
        fp->fv.kb.hat.x0 = NULL;
    }
}

void ftKb_SpecialS_Enter(Fighter_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    fp->cmd_vars[0] = 0;
    Fighter_ChangeMotionState(gobj, ftKb_MS_SpecialS, 0, 0.0f, 1.0f, 0.0f,
                              NULL);
    ftAnim_8006EBA4(gobj);
    fp->accessory4_cb = &fn_800F53AC;
}

void ftKb_SpecialAirS_Enter(Fighter_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    ftKb_DatAttrs* da = fp->dat_attrs;
    PAD_STACK(8);
    fp->cmd_vars[0] = 0;
    if (!fp->fv.kb.x64) {
        fp->self_vel.y = da->specials_aerial_vertical_momentum;
        fp->fv.kb.x64 = true;
    }
    Fighter_ChangeMotionState(gobj, ftKb_MS_SpecialAirS, 0, 0.0f, 1.0f, 0.0f,
                              NULL);
    ftAnim_8006EBA4(gobj);
    fp->accessory4_cb = &fn_800F53AC;
}

void ftKb_SpecialS_Anim(Fighter_GObj* gobj)
{
    if (ftAnim_IsFramesRemaining(gobj) == 0) {
        ft_8008A2BC(gobj);
    }
}

void ftKb_SpecialAirS_Anim(Fighter_GObj* gobj)
{
    PAD_STACK(0x08);
    if (ftAnim_IsFramesRemaining(gobj) == 0) {
        ftCo_Fall_Enter(gobj);
    }
}

void ftKb_SpecialS_Phys(Fighter_GObj* gobj)
{
    ft_80084F3C(gobj);
}

void ftKb_SpecialAirS_Phys(Fighter_GObj* gobj)
{
    ft_80084EEC(gobj);
}

void ftKb_SpecialS_Coll(Fighter_GObj* gobj)
{
    Fighter* fp;
    if (ft_80082708(gobj) != 0) {
        return;
    }
    fp = GET_FIGHTER(gobj);
    if (fp->fv.kb.hat.x0 != NULL) {
        it_802ADC34(fp->fv.kb.hat.x0);
        fp->fv.kb.hat.x0 = NULL;
    }
    ftCo_Fall_Enter(gobj);
}

void ftKb_SpecialAirS_Coll(Fighter_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    ftKb_DatAttrs* da = fp->dat_attrs;
    if (ft_80081D0C(gobj)) {
        Fighter* fp2 = GET_FIGHTER(gobj);
        if (fp2->fv.kb.hat.x0 != NULL) {
            it_802ADC34(fp2->fv.kb.hat.x0);
            fp2->fv.kb.hat.x0 = NULL;
        }
        fp->fv.kb.x64 = false;
        ftCo_LandingFallSpecial_Enter(gobj, 0, da->specials_landing_lag);
    }
}
