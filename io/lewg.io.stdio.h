#ifndef LEWG_IO_STDIO_H_
  #define LEWG_IO_STDIO_H_

  #include <errno.h>

  #include <stdio.h>
  #include <unistd.h>

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"

  lewgReturn_t lewgStdInReadln(char* buffer, const int len);
  lewgReturn_t lewgStdOutPrint(char* format, ...);

#endif