#include "ftCo_DownAttack.h"

#include "ftCo_HammerWait.h"
#include "ftCo_Passive.h"

#include <platform.h>

#include "ft/fighter.h"
#include "ft/ft_081B.h"
#include "ft/ft_084E.h"
#include "ft/ft_0892.h"
#include "ft/ft_0DF1.h"
#include "ft/ftanim.h"
#include "ft/types.h"
#include "ftCommon/types.h"
#include "it/types.h"
#include "lb/types.h"

#include <common_structs.h>
#include <baselib/gobj.h>

static itECB it_ecb = { 14, 0, -3, +3 };

static inline bool inlineA0(Fighter* fp)
{
    if (fp->input.x668 & (HSD_PAD_A | HSD_PAD_B)) {
        return true;
    }
    return false;
}

int ftCo_800984D4(Fighter_GObj* gobj)
{
    Fighter* fp = gobj->user_data;
    if (inlineA0(fp) || ftCo_800DF644(fp)) {
        bool msid = fp->motion_id == ftCo_MS_DownWaitU ? ftCo_MS_DownAttackU
                                                       : ftCo_MS_DownAttackD;
        ftCo_8009856C(gobj, msid);
        return true;
    }
    return false;
}

void ftCo_8009856C(Fighter_GObj* gobj, FtMotionId msid)
{
    Fighter_ChangeMotionState(gobj, msid, Ft_MF_None, 0, 1, 0, NULL);
    ftAnim_8006EBA4(gobj);
}

void ftCo_DownAttack_Anim(Fighter_GObj* gobj)
{
    if (!ftAnim_IsFramesRemaining(gobj)) {
        ft_8008A2BC(gobj);
    }
}

void ftCo_DownAttack_IASA(Fighter_GObj* gobj) {}

void ftCo_DownAttack_Phys(Fighter_GObj* gobj)
{
    ft_80084F3C(gobj);
}

void ftCo_DownAttack_Coll(Fighter_GObj* gobj)
{
    ft_80084104(gobj);
}

void ftCo_80098634(struct Item_FtTrack* track)
{
    Fighter_GObj* cur_gobj;
    int i;
    for (cur_gobj = HSD_GObj_Entities->fighters, i = 0; cur_gobj != NULL;
         cur_gobj = cur_gobj->next, i++)
    {
        Fighter* fp = cur_gobj->user_data;
        track->x0_ecb_arr[i].top = it_ecb.top;
        track->x0_ecb_arr[i].bottom = it_ecb.bottom;
        track->x0_ecb_arr[i].right = it_ecb.right;
        track->x0_ecb_arr[i].left = it_ecb.left;
        track->xC0_pos_arr[i] = fp->cur_pos;
    }
    track->x150_count = i;
}

bool ftCo_800986B0(Fighter_GObj* gobj)
{
    Fighter* fp = GET_FIGHTER(gobj);
    if (!ftCo_800C5240(gobj) && fp->x680 < p_ftCommonData->x250 &&
        fp->x684 >= p_ftCommonData->x1C)
    {
        return true;
    }
    return false;
}

int ftCo_8009872C(Fighter_GObj* gobj)
{
    u8 _[8] = { 0 };
    if (ftCo_800986B0(gobj)) {
        ftCo_800987D0(gobj);
        return true;
    }
    return false;
}
