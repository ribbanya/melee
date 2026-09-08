#include <Runtime/platform.h>

#include <stdio.h>

#include "rprecord.h"
#include <fray/lb/lbqol.h>
#include <melee/if/textdraw.h>
#include <melee/if/textlib.h>
#include <melee/if/types.h>

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
    FRAY_ASSERT(text);
    DevText_Show(gobj, text);
    DevText_HideCursor(text);
    DevText_SetBGColor(text, bg);
    DevText_SetTextColor(text, fg);
    DevText_SetScale(text, TEXT_SCALE * TEXT_RATIO, TEXT_SCALE);
}

static void fmtButtons(ReplayFrame* rf, char dst[8])
{
    char const empty = ' ';
    dst[0] = rf->in.a ? 'A' : empty;
    dst[1] = rf->in.b ? 'B' : empty;
    dst[2] = rf->in.x ? 'X' : empty;
    dst[3] = rf->in.y ? 'Y' : empty;
    dst[4] = rf->in.l ? 'L' : empty;
    dst[5] = rf->in.r ? 'R' : empty;
    dst[6] = rf->in.z ? 'Z' : empty;
    dst[7] = rf->in.dpad_up ? '^' : empty;
}

static void fmtFlags(ReplayFrame* rf, char dst[4])
{
    char const empty = ' ';
    dst[0] = rf->out.facing_left ? 'L' : empty;
    dst[1] = rf->out.airborne ? 'A' : empty;
    dst[2] = rf->out.ecb_locked ? 'E' : empty;
    dst[3] = rf->out.hit_this_frame ? 'X' : empty;
}

static float convertCoord(s8 val, bool is_cpu)
{
    float tmp = val;
    if (is_cpu) {
        tmp = (s8) (tmp / 1.5875f);
    }
    return tmp / 80;
}

void ReplayText_Update(void)
{
    size_t i;
    char buttons[8];
    char flags[4];

    DevText_Erase(text);
    DevText_SetCursorXY(text, 0, 0);

    for (i = 0; i < 2; i++) { /// @todo get len from lib
        ReplayFrame* rf = Replay_GetCurrentFrame(i);
        fmtButtons(rf, buttons);
        fmtFlags(rf, flags);

        if (i > 0) {
            DevText_Print(text, "\n");
        }
        DevText_Printf(text,
                       "(%+.4f,%+.4f) (%+.4f,%+.4f) %3d\n"
                       "%.8s | %.4s",
                       convertCoord(rf->in.lstick.x, true),
                       convertCoord(rf->in.lstick.y, true),
                       convertCoord(rf->in.cstick.x, true),
                       convertCoord(rf->in.cstick.y, true), rf->in.trigger,
                       buttons, flags);
    }
}
