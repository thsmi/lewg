
#include "./../utils/lewg.core.daemon.h"

lewgReturn_t lewgInitDaemon(lewgSingleton_t* singleton, int force)
{
  lewgReturn_t rv = LEWG_ERROR;
  
  // First we need to ensure no concurrent deamon is running...
  // ... otherwise our startup ensup in an epic fail.
  rv = lewgInitSingleton(singleton, force);
  
  LEWG_ENSURE_SUCCESS(rv);

  if (daemon(0, 0) == -1)
  {
    lewgLogError("Cannot daemonize\n");
    lewgDestroyDaemon(singleton);

    return LEWG_ERROR;
  }

  lewgLogInfo("Process successfully deamonized\n");

  lewgUpdateSingleton(singleton);

  lewgLogInfo("Process successfully locked \n");

  return LEWG_SUCCESS;
}

lewgReturn_t lewgDestroyDaemon(lewgSingleton_t* singleton)
{
  lewgDestroySingleton(singleton);
  return LEWG_SUCCESS;
}
