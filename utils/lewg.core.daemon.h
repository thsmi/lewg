#ifndef LEWG_CORE_DAEMON_H_
  #define LEWG_CORE_DAEMON_H_

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"
  #include "./../utils/lewg.core.singleton.h"

  // required for daemon
  #include <unistd.h> 
  // required for errno
  #include <errno.h>

  lewgReturn_t lewgInitDaemon(lewgSingleton_t* singleton, int force);
  lewgReturn_t lewgDestroyDaemon(lewgSingleton_t* singleton);

#endif
