
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./../utils/lewg.core.types.h"
#include "./../utils/lewg.util.logger.h"
#include "./../utils/lewg.core.entrypoint.h"
#include "./../utils/lewg.core.daemon.h"
#include "./../utils/lewg.core.args.h"

static lewgReturn_t lewgRun()
{
  lewgReturn_t rv = LEWG_ERROR;

  rv = onMeeetInit();

  if (rv != LEWG_SUCCESS)
  {
    lewgLogError("Exiting. Initialization failed\n");
    return rv;
  }

  // in case of signals we should quit
  do {
    rv = onMeeetStep();
  } while (rv == LEWG_CONTINUE);

  return rv;
}

static lewgReturn_t lewgDestroy()
{
  lewgReturn_t rv = LEWG_ERROR;

  // we ignore any errors during shutdown...
  rv = onMeeetDestroy();

  if (rv != LEWG_SUCCESS)
    lewgLogInfo("Cleanup failed...\n ");
  else
    lewgLogInfo("Shutdown completed...\n ");


  return rv;
}

int main(int argc, char** argv)
{
  lewgReturn_t rv = LEWG_ERROR;

  lewgInitLog();
  lewgLogInfo("\n");
  lewgLogInfo("Initializing ... \n");
  lewgLogInfo("\n");
  
  lewgInitArguments(argc, argv);

  // Parse for special startup modes arguments ...
  rv = lewgGetArgument("stop");

  if (rv == LEWG_SUCCESS)
  {
    printf("Stopping all instances......");
    // To stop a daemon, we kill the daemon's  singleton.
    rv = lewgKillSingleton();
    
    if (rv != LEWG_SUCCESS)
    {
      printf("[failed]\n");
      return EXIT_FAILURE;
    }

    printf("[ok]\n");
    return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
  }
  
  rv = lewgGetArgument("help");
  
  if (rv == LEWG_SUCCESS)
  {
    onMeeetHelp();
    
    return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
  }

  // ... normal startup
  lewgSingleton_t singleton;

  // in case we should start as daemon we need some extra magic..
  rv = lewgGetArgument("start");

  if (rv == LEWG_SUCCESS)
  {
    printf("Starting daemon...\n");

    // A Startup may be enforced. This means it will replace ...
    // ... an existing daemon.
    rv = lewgGetArgument("force");
    int force = ( rv == LEWG_SUCCESS)? 1: 0;

    // A daemon is by definition a singleton. It will invoke...
    // ... lewgInitSingleton as soon as the daemon reaches pid 1
    rv = lewgInitDaemon(&singleton, force);

    if (rv != LEWG_SUCCESS)
    {
      printf("          ... failed\n");
      return EXIT_FAILURE;
    }
    
    rv =  lewgRun();

    lewgDestroy();
    lewgDestroyDaemon(&singleton);

    return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
  }

  rv = lewgGetArgument("status");
  if (rv == LEWG_SUCCESS)
  {
    
     rv = lewgIsSingleton();
     
     if (rv == LEWG_SUCCESS)
       printf("Daemon status: terminated\n");
     else
       printf("Daemon status: running\n");

     return EXIT_SUCCESS;
  }
  
  printf("Unknow command \n status, stop, start, force start, help\n");

  return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
}
