#include "melee/if/textdraw.h"
#include "melee/if/textlib.h"
#include "melee/if/types.h"
#include "melee/pl/player.h"

static DevText* text;
static char buf[0x400];

void ReplayText_Setup(void)
{
    HSD_GObj* gobj = DevText_GetGObj();
    text = DevText_Create(1, 20, 20, 60, 7, buf);
    if (text) {
        GXColor bg = { 0x00, 0x00, 0x00, 0x00 };
        GXColor fg = { 0xFF, 0xFF, 0xFF, 0xFF };
        DevText_Show(gobj, text);
        DevText_HideCursor(text);
        DevText_SetBGColor(text, bg);
        DevText_SetTextColor(text, fg);
        DevText_SetScale(text, 12.0F, 12.0F);
    }
}

void ReplayText_Update(void)
{
    StaticPlayer* player;
    s32 slot;

    DevText_Erase(text);
    DevText_SetCursorXY(text, 0, 0);
    DevText_Printf(text, "A B  C D E    F    G");
    for (slot = 0; slot < 6; slot++) {
        player = Player_GetPtrForSlot(slot);
        DevText_Printf(text, "\n%d %d %2d %d %2.2f %2.2f %2.2f",
                       player->player_state, player->cpu_level,
                       player->cpu_type, player->handicap, player->unk50,
                       player->attack_ratio, player->defense_ratio);
    }
}
