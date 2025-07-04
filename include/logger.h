#ifndef HFX_LOGGER_H
#define HFX_LOGGER_H

#include <stdio.h>

enum
{
  LOG_DEBUG,
  LOG_TRACE,
  LOG_WARNING,
  LOG_ERROR,
};

void HFX_LogInit(
  const FILE *const fp);

void HFX_Log(
  const char *format, ...);

const char* HFX_LogLevelCStr(
  int level);

#endif //HFX_LOGGER_H
