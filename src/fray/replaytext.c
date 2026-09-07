#include "melee/gm/forward.h"
#include "melee/if/textdraw.h"
#include "melee/if/textlib.h"
#include "melee/if/types.h"
#include "melee/pl/player.h"

#define TEXT_WIDTH 60
#define TEXT_HEIGHT 7

static DevText* text;
static char buf[TEXT_WIDTH * TEXT_HEIGHT];

void ReplayText_Setup(void)
{
    HSD_GObj* gobj = DevText_GetGObj();
    text = DevText_Create(1, 5, 5, TEXT_WIDTH, TEXT_HEIGHT, buf);
    if (text) {
        GXColor bg = { 0x00, 0x00, 0x00, 0x00 };
        GXColor fg = { 0xFF, 0xFF, 0xFF, 0xFF };
        DevText_Show(gobj, text);
        DevText_HideCursor(text);
        DevText_SetBGColor(text, bg);
        DevText_SetTextColor(text, fg);
        DevText_SetScale(text, 18.0f, 24.0f);
    }
}

void ReplayText_Update(void)
{
    StaticPlayer* player;
    ssize_t i;

    DevText_Erase(text);
    DevText_SetCursorXY(text, 0, 0);
    DevText_Printf(text, "A B  C D E    F    G");
    for (i = 0; i < Gm_Player_NumMax; i++) {
        player = Player_GetPtrForSlot(i);
        DevText_Printf(text, "\n%d %d %2d %d %2.2f %2.2f %2.2f",
                       player->player_state, player->cpu_level,
                       player->cpu_type, player->handicap, player->unk50,
                       player->attack_ratio, player->defense_ratio);
    }
}
