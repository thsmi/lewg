
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "./../utils/lewg.core.types.h"
#include "./../utils/lewg.util.logger.h"
#include "./../utils/lewg.core.entrypoint.h"
#include "./../utils/lewg.core.args.h"

static lewgReturn_t lewgRun()
{
  lewgReturn_t rv = LEWG_ERROR;

  rv = onMeeetInit();

  if (rv != LEWG_SUCCESS)
  {
    lewgLogError("Terminating, initialization failed\n");
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
    lewgLogInfo("Application terminated...\n ");

  return rv;
}

int main(int argc, char** argv)
{
  lewgReturn_t rv = LEWG_ERROR;

  lewgInitLog();
  lewgLogInfo("\n");
  lewgLogInfo("Initializing Application... \n");
  lewgLogInfo("\n");
  
  lewgInitArguments(argc, argv);

  rv = lewgGetArgument("help");

  if (rv == LEWG_SUCCESS)
  {
    onMeeetHelp();
    return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
  }

  // Start our life cycle...
  rv =  lewgRun();

  lewgDestroy();

  return (rv == LEWG_SUCCESS)?EXIT_SUCCESS:EXIT_FAILURE;
}
