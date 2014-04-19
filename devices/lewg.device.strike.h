#ifndef LEWG_DEVICE_STRIKE_H_
  #define LEWG_DEVICE_STRIKE_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "../utils/lewg.core.types.h"
  #include "../io/lewg.io.serial.h"
  #include "../utils/lewg.util.logger.h"

  lewgReturn_t lewgStrikeInit(int* fd, const char* device);
  lewgReturn_t lewgStrikeOpen(int fd);
  lewgReturn_t lewgStrikeDestroy(int* fd);

#endif

