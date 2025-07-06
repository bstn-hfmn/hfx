#include "common.h"

static const char* g_Error = "";


void HFX_SetLastError(const char* error)
{
    g_Error = error;
}

const char* HFX_GetLastError()
{
    return g_Error;
}
