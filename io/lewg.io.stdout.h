#ifndef LEWG_IO_STDOUT_H_
  #define LEWG_IO_STDOUT_H_

  #include <errno.h>

  #include <stdio.h>
  #include <unistd.h>

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"

  lewgReturn_t lewgStdOutWriteln(char* buffer, const int len);

#endif