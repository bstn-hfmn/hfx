#include <logger.h>
#include <stdarg.h>

void hfx_log_init(const char* path)
{
    if (path == nullptr)
    {
        gLogger.file = stdout;
        return;
    }
}

void hfx_log(const char *format, ...)
{
    if (gLogger.file == nullptr)
        hfx_log_init(nullptr);

    va_list args;
    va_start(args, format);

    vfprintf(gLogger.file, format, args);
    va_end(args);
}

const char* hfx_log_level_cstr(const LogLevel level)
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
