#include "melee/ft/kinds/ftCommon/ftCo_0A01.h"
#include "melee/gm/forward.h"
#include "melee/if/textdraw.h"
#include "melee/if/textlib.h"
#include "melee/if/types.h"
#include "melee/pl/player.h"
#include "sysdolphin/baselib/debug.h"

#define TEXT_X 5
#define TEXT_Y 5
#define TEXT_WIDTH 60
#define TEXT_HEIGHT 7

static DevText* text;
static char buf[TEXT_WIDTH * TEXT_HEIGHT * 2];

void ReplayText_Setup(void)
{
    GXColor bg = { 0x00, 0x00, 0x00, 0x00 };
    GXColor fg = { 0xFF, 0xFF, 0xFF, 0xFF };
    HSD_GObj* gobj = DevText_GetGObj();
    text = DevText_Create(1, TEXT_X, TEXT_Y, TEXT_WIDTH, TEXT_HEIGHT, buf);
    HSD_ASSERT(__LINE__, text);
    DevText_Show(gobj, text);
    DevText_HideCursor(text);
    DevText_SetBGColor(text, bg);
    DevText_SetTextColor(text, fg);
    DevText_SetScale(text, 18.0f, 24.0f);
}

void ReplayText_Update(void)
{
    HSD_GObj* gobj;
    Fighter* fp;
    HSD_Pad buttons;
    Gm_PKind pkind;
    Vec2 lstick;
    Vec2 cstick;
    float triggers;
    size_t i;

    DevText_Erase(text);
    DevText_SetCursorXY(text, 0, 0);

    for (i = 0; i < Gm_Player_NumMax; i++) {
        pkind = Player_GetPlayerSlotType(i);

        if (pkind == Gm_PKind_NA) {
            continue;
        }

        buttons = 0;
        gobj = Player_GetEntity(i);
        HSD_ASSERT(__LINE__, gobj);
        fp = gobj->user_data;
        HSD_ASSERT(__LINE__, fp);

        switch (pkind) {
        case Gm_PKind_Human:
        case Gm_PKind_Cpu:
            buttons = fp->input.held_buttons[0];
            lstick = fp->input.lstick[0];
            cstick = fp->input.cstick[0];
            triggers = fp->input.triggers[0];
            break;
        //     buttons = ftCo_GetCpuButtons(fp);
        //     lstick.x = ftCo_GetCpuLStickX(fp);
        //     lstick.y = ftCo_GetCpuLStickY(fp);
        //     cstick.x = ftCo_GetCpuCStickX(fp);
        //     cstick.y = ftCo_GetCpuCStickY(fp);
        //     // ftCo_GetCpuLTrigger(Fighter *fp)
        //     break;
        case Gm_PKind_NA:
        case Gm_PKind_Demo:
        case Gm_PKind_Boss:
            HSD_ASSERTREPORT(__LINE__, 0, "Unexpected PKind %d!", pkind);
        }

        if (i > 0) {
            DevText_Print(text, "\n");
        }
        DevText_Printf(text, "%d: (%.2f,%.2f) (%.2f) %08x", i, lstick.x,
                       lstick.y, cstick.x, cstick.y, triggers, buttons);
    }
}
