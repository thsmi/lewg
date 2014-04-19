#ifndef LEWG_CODE_LOCAL_STORE_H_
  #define LEWG_CODE_LOCAL_STORE_H_

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  #include <ctype.h>

  #include "./../utils/lewg.util.logger.h"
  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.core.string.h"
  #include "./../io/lewg.io.file.h"

  #ifndef LEWG_MAX_CODE_LENGTH
    #define LEWG_MAX_CODE_LENGTH 12
  #endif

  typedef struct {
    char code[LEWG_MAX_CODE_LENGTH];
    char type;
  } lewgLocalCode_t;

  lewgReturn_t lewgSanatizeCode(const char* buffer, lewgLocalCode_t* code);
  lewgReturn_t lewgValidateLifeCycle();

#endif