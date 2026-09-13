#include <dolphin/os.h>
#include <melee/lb/lbcardnew.h>

typedef struct {
    char filename[64];
} ReplayCardData;

static ReplayCardData data;

void ReplayCard_Init(void)
{
    OSTime ticks = OSGetTime();
    char* text = "Fray Seeded Bisimulation";
    OSCalendarTime time;
    OSTicksToCalendarTime(OSSecondsToTicks(OSTicksToSeconds(ticks)), &time);
    memset(data.filename, 0, sizeof(data.filename));
    sprintf(data.filename, "%s %04d-%02d-%02d %02d:%02d:%02d", text, time.year,
            time.mon + 1, time.mday, time.hour, time.min, time.sec);

    lbCardNew_AllocWorkArea();
    OSReport("%s", data.filename);
}
