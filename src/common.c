#include "common.h"

static HFX_ERROR g_Error = 0;


void HFX_SetLastError(HFX_ERROR error)
{
    g_Error = error;
}

HFX_ERROR HFX_GetLastError()
{
    return g_Error;
}
