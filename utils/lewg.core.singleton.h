#ifndef LEWG_CORE_SINGLETON_H_
  #define LEWG_CORE_SINGLETON_H_

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"

  // required for errno
  #include <errno.h>
  // required for pid related stuff
  #include <bsd/libutil.h>
  // required by kill
  #include <signal.h>

  typedef struct {
    struct pidfh* handle;
  } lewgSingleton_t;

  lewgReturn_t lewgInitSingleton(lewgSingleton_t* singleton, int force);
  lewgReturn_t lewgUpdateSingleton(lewgSingleton_t* singleton);
  lewgReturn_t lewgDestroySingleton(lewgSingleton_t* singleton);

  lewgReturn_t lewgIsSingleton();
  lewgReturn_t lewgKillSingleton();

#endif
