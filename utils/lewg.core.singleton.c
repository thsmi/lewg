
#include "./../utils/lewg.core.singleton.h"

#ifndef LEWG_PIDFILE
  #define LEWG_PIDFILE 0
  //"/var/run/lewg.pid"
#endif

lewgReturn_t lewgInitSingleton(lewgSingleton_t* handle, int force)
{
  pid_t otherpid;

  handle->handle = pidfile_open(LEWG_PIDFILE, 0600, &otherpid);

  if (handle->handle != NULL)
    return LEWG_SUCCESS;

  if (errno == EEXIST)
    lewgLogError("Daemon already running, pid: %jd.\n", (intmax_t)otherpid);
  else
    lewgLogError("Cannot open or create pidfile\n"); 

  if (force)
  {
    lewgLogInfo("Killing process pid %jd.\n",(intmax_t)otherpid);
    kill(otherpid, SIGINT);

    return lewgInitSingleton(handle,0);
  }

  return LEWG_ERROR;
}

lewgReturn_t lewgKillSingleton()
{
  lewgSingleton_t singleton;
  lewgReturn_t rv = LEWG_ERROR;

  // first we steal the lock with a force tag. So that the ...
  // ... singleton will be enforced and other the other instance...
  // ... will be closed. 
  rv = lewgInitSingleton(&singleton, 1);
  
  LEWG_ENSURE_SUCCESS(rv);
  //if (rv != LEWG_SUCCESS)
  //  return rv;

  // ... then we release the lock
  rv = lewgDestroySingleton(&singleton);

  return rv;
}

lewgReturn_t lewgIsSingleton()
{
  lewgSingleton_t singleton;
  lewgReturn_t rv = LEWG_ERROR;

  // it's no  singleton if lock failes ...
  rv = lewgInitSingleton(&singleton, 0);
  
  LEWG_ENSURE_SUCCESS(rv);
  //if (rv != LEWG_SUCCESS)
  //  return rv;

  // ... it worked, so it's a singleton and ...
  // ... we need to release the lock.
  lewgDestroySingleton(&singleton);

  return LEWG_SUCCESS;
}

lewgReturn_t lewgUpdateSingleton(lewgSingleton_t* handle)
{
  pidfile_write(handle->handle);
  
  return LEWG_SUCCESS;
}

lewgReturn_t lewgDestroySingleton(lewgSingleton_t* handle)
{
  pidfile_remove(handle->handle);
  return LEWG_SUCCESS;
}
