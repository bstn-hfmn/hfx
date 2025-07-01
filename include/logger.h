#ifndef HFX_LOGGER_H
#define HFX_LOGGER_H

#include <common.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HFX_LOG(level, format, ...) \
  hfx_log("[%s][%s:%d]: " format, hfx_log_level_cstr(level), __FILE__, __LINE__, ##__VA_ARGS__)

static struct {
  FILE* file;
} gLogger;

HFX_API typedef enum {
  LOG_DEBUG,
  LOG_TRACE,
  LOG_WARNING,
  LOG_ERROR,
} LogLevel;

HFX_API void hfx_log_init(
  const char* path);

HFX_API void hfx_log(
  const char *format, ...);

HFX_API const char* hfx_log_level_cstr(
  const LogLevel level);

#ifdef __cplusplus
}
#endif

#endif //HFX_LOGGER_H
