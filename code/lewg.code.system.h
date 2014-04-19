#ifndef LEWG_CODE_SYSTEM_H
  #define LEWG_CODE_SYSTEM_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "./../utils/lewg.util.logger.h"
  #include "./../utils/lewg.core.types.h"

  lewgReturn_t lewgIsResetCode(const char* buffer, int length);
  lewgReturn_t lewgIsShutdownCode(const char* buffer, int length);
  

#endif