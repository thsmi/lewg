#ifndef LEWG_UTIL_SYSTEM_H_
  #define LEWG_UTIL_SYSTEM_H_
  
  #include <stdlib.h>
  #include "lewg.core.types.h"
  #include "lewg.util.logger.h"
  
  lewgReturn_t lewgReboot();
  lewgReturn_t lewgShutdown();
  
  lewgReturn_t lewgSystemCall();
#endif