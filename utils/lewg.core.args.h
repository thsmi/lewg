#ifndef LEWG_CORE_ARGS_H_
  #define LEWG_CORE_ARGS_H_

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"

  // Require by strlen and memcmp
  #include <string.h>

  lewgReturn_t lewgInitArguments(int argc, char** argv);
  void lewgDestroyArguments();
  
  lewgReturn_t lewgGetArgument(const char* option);
  lewgReturn_t lewgGetStrArgument(const char* option, char** arg);
  lewgReturn_t lewgGetIntArgument(const char* options, int* arg);
  

#endif
