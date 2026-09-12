#include <Runtime/platform.h>

#include <stdio.h>

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
}
