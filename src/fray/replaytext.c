#include <stdio.h>

#include "melee/gm/forward.h"
#include "melee/if/textdraw.h"
#include "melee/if/textlib.h"
#include "melee/if/types.h"
#include "melee/pl/forward.h"
#include "melee/pl/player.h"
#include "Runtime/platform.h"
#include "sysdolphin/baselib/debug.h"

#define TEXT_X 2
#define TEXT_Y 2
#define TEXT_SCALE 18
#define TEXT_RATIO 0.8f
#define TEXT_WIDTH 60
#define TEXT_HEIGHT 7

static DevText* text;
static char buf[TEXT_WIDTH * TEXT_HEIGHT * 2];

void ReplayText_Setup(void)
{
    static GXColor const bg = { 0x00, 0x00, 0x00, 0x00 };
    static GXColor const fg = { 0xFF, 0xFF, 0xFF, 0xFF };
    HSD_GObj* gobj = DevText_GetGObj();
    text = DevText_Create(1, TEXT_X, TEXT_Y, TEXT_WIDTH, TEXT_HEIGHT, buf);
    HSD_ASSERT(__LINE__, text);
    DevText_Show(gobj, text);
    DevText_HideCursor(text);
    DevText_SetBGColor(text, bg);
    DevText_SetTextColor(text, fg);
    DevText_SetScale(text, TEXT_SCALE * TEXT_RATIO, TEXT_SCALE);
}

static inline float convertCpuCoord(s8 val)
{
    return ((s8) (val / 1.5875f)) / 80.0f;
}

void ReplayText_Update(void)
{
    HSD_GObj* gobj;
    Fighter* fp;
    HSD_Pad buttons;
    s8 lstick_x, lstick_y;
    s8 cstick_x, cstick_y;
    s8 triggers;
    size_t i;

    DevText_Erase(text);
    DevText_SetCursorXY(text, 0, 0);

    for (i = 0; i < Gm_Player_NumMax; i++) {
        Gm_PKind pkind = Player_GetPlayerSlotType(i);
        /// @todo Look up ::HSD_PadStatus for humans
        if (pkind != Gm_PKind_Cpu) {
            HSD_ASSERTMSG(__LINE__, pkind != Gm_PKind_Human,
                          "Human recording not implemented!");
            continue;
        }

        buttons = 0;
        gobj = Player_GetEntity(i);
        HSD_ASSERT(__LINE__, gobj);
        fp = gobj->user_data;
        HSD_ASSERT(__LINE__, fp);

        buttons = fp->cpu.buttons;
        lstick_x = fp->cpu.lstickX;
        lstick_y = fp->cpu.lstickY;
        cstick_x = fp->cpu.cstickX;
        cstick_y = fp->cpu.cstickY;
        triggers = MAX(fp->cpu.ltrigger, fp->cpu.rtrigger);

        if (i > 0) {
            DevText_Print(text, "\n");
        }
        DevText_Printf(text, "%d: (%+.4f,%+.4f) (%+.4f,%+.4f) %d %08x",
                       fp->player_id, convertCpuCoord(lstick_x),
                       convertCpuCoord(lstick_y), convertCpuCoord(cstick_x),
                       convertCpuCoord(cstick_y), triggers, buttons);
    }
}
