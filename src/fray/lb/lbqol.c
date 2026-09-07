#include <Runtime/platform.h>

#include <sysdolphin/baselib/debug.h>

static int logPassthrough(UNUSED __file_handle arg0,
                          UNUSED unsigned char* arg1, UNUSED size_t* arg2,
                          UNUSED __idle_proc arg3)
{
    return 0;
}

void reportPassthrough(UNUSED const unsigned char* str, UNUSED size_t len) {}

/// Shut up character-by-character ::OSReport spam.
/// @todo Every ::OSReport is still doubled under Dolphin
void Qol_LogInit(void)
{
    HSD_SetReportCallback(reportPassthrough);

    // Will crash if null.
    stdout->write_proc = logPassthrough;
}
