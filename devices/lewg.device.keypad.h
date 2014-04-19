#ifndef LEWG_DEVICE_KEYPAD_H_
  #define LEWG_DEVICE_KEYPAD_H_

  #include "./../utils/lewg.util.logger.h"
  #include "./../io/lewg.io.serial.h"
  #include <string.h>
  #include <unistd.h>
  #include <stdio.h>

  lewgReturn_t lewgKeyPadInit(int* fd, const char* device);

  lewgReturn_t lewgKeyPadGetCode(int* fd, char* buffer, int buflen);

  lewgReturn_t lewgKeyPadDestroy(int* fd);

#endif
