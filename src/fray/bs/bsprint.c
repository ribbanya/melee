#include <Runtime/platform.h>

int bsPrint_Var(char* s, const char* name, void* v, int indent, bool newline)
{
}

int bsPrint_Bool(char* s, const char* name, bool v)
{
    const char t[] = "true";
    const char f[] = "false";
    return sprintf(s, "%s: %s");
}
