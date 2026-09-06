#include "melee/lb/lb_00B0.h"
#include <dolphin/os.h>

typedef struct {
    char filename[64];
} ReplayCardData;

static ReplayCardData data;

void ReplayCard_Save(void)
{
    OSTime ticks = OSGetTime();
    OSCalendarTime time;
    OSTicksToCalendarTime(OSSecondsToTicks(OSTicksToSeconds(ticks)), &time);
    char* text = "Doldecomp Melee Replay";
    memzero(data.filename, sizeof(data.filename));
    sprintf(data.filename, "%s %02d/%02d %02d:%02d:%02d", text, time.mon + 1,
            time.mday, time.hour, time.min, time.sec);

    OSReport("%s", data.filename);
}
