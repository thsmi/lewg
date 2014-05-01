#ifndef LEWG_UTIL_LOGGER_H
  #define LEWG_UTIL_LOGGER_H

  #include "lewg.core.types.h"
  #include <stdarg.h>
  #include <time.h>
  #include <stdlib.h>

  typedef enum {
    LEWG_LOG_INFO,
    LEWG_LOG_ERROR
  } lewgLogType_t;

  #define lewgLogInfo(...)  lewgLog(LEWG_LOG_INFO, __VA_ARGS__ )
  #define lewgLogError(...) lewgLog(LEWG_LOG_ERROR, __VA_ARGS__ )

  lewgReturn_t lewgInitLog();
  void lewgDestroyLog();

  lewgReturn_t  lewgLog(lewgLogType_t type, const char* format, ...);

#endif
