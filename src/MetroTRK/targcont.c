#include "targcont.h"

#include <platform.h>

#include "MetroTRK/dolphin_trk_glue.h"
#include "MetroTRK/targimpl.h"

DSError TRKTargetContinue(void)
{
    TRKTargetSetStopped(false);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return kNoError;
}
