#ifndef LEWG_CORE_STRING_H_
  #define LEWG_CORE_STRING_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #include <ctype.h>

  #include "./../utils/lewg.util.logger.h"
  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.system.h"

  lewgReturn_t lewgIsWhiteSpace(const char* buffer);
  lewgReturn_t lewgIsComment(const char* buffer);
  lewgReturn_t lewgParseTime(const char* buffer, struct tm* timeinfo);
  lewgReturn_t lewgParseInterval(const char* buffer, time_t* begin, time_t* end);

#endif