#ifndef LEWG_UTIL_SCRIPT_H_
  #define LEWG_UTIL_SCRIPT_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "./../utils/lewg.util.logger.h"
  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.system.h"
  #include "./../io/lewg.io.file.h"
  #include "./../utils/lewg.core.string.h"

  lewgReturn_t lewgExecuteScript(const char* file);


#endif