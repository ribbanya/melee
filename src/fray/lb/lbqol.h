#ifndef FRAY_LB_LBQOL_H
#define FRAY_LB_LBQOL_H

#include <sysdolphin/baselib/debug.h>

#define FRAY_ASSERT(cond) HSD_ASSERT(__LINE__, cond)
#define FRAY_ASSERTMSG(cond, msg) HSD_ASSERTMSG(__LINE__, cond, msg)
#define FRAY_ASSERTREPORT(cond, ...)                                          \
    HSD_ASSERTREPORT(__LINE__, cond, __VA_ARGS__)

void Qol_LogInit(void);

#endif
