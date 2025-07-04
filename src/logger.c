#include "logger.h"
#include <stdarg.h>

static struct {
    FILE* file;
} g_Logger;

void HFX_LogInit(
    const FILE *const fp)
{
    if (g_Logger.file != nullptr)
        return;

    if (fp == nullptr)
    {
        g_Logger.file = stderr;
        HFX_Log( "[ERROR]: Invalid file pointer passed to logger\n");
    } else {
        g_Logger.file = (FILE*)fp;
    }
}

void HFX_Log(
    const char *const format, ...)
{
    if (g_Logger.file == nullptr)
        return;

    va_list args;
    va_start(args, format);

    vfprintf(g_Logger.file, format, args);
    va_end(args);
}

const char* HFX_LogLevelCStr(
    const int level)
{
    switch (level)
    {
        case LOG_DEBUG: return "DEBUG";
        case LOG_TRACE: return "TRACE";
        case LOG_ERROR: return "ERROR";
        case LOG_WARNING: return "WARNING";
        default: return "UNK";
    }
}
