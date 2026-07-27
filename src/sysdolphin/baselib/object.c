#include "object.h"

#include <baselib/types.h>

#include <baselib/class.h>

HSD_ClassInfo hsdObj = { ObjInfoInit };
extern HSD_ClassInfo hsdClass;

void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj",
                     sizeof(HSD_ObjInfo), sizeof(HSD_Obj));
}
