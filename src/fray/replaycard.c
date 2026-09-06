#include <dolphin/os.h>
#include <melee/lb/lb_00B0.h>

typedef struct {
    char filename[64];
} ReplayCardData;

static ReplayCardData data;

void ReplayCard_Save(void)
{
    OSTime ticks = OSGetTime();
    char* text = "Fray Debug Replay";
    OSCalendarTime time;
    OSTicksToCalendarTime(OSSecondsToTicks(OSTicksToSeconds(ticks)), &time);
    memzero(data.filename, sizeof(data.filename));
    sprintf(data.filename, "%s %04d-%02d-%02d %02d:%02d:%02d", text, time.year,
            time.mon + 1, time.mday, time.hour, time.min, time.sec);

    OSReport("%s", data.filename);
}
