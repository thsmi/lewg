#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../devices/lewg.device.strike.h"
#include "./../../utils/lewg.util.logger.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"

#ifndef LEWG_DEV_USB
  #define LEWG_DEV_USB "/dev/ttyUSB0"
#endif

#define LEWG_STRIKE_ARG   "--strike="
#define LEWG_ACTION_ARG    "open"

static char* strike = LEWG_DEV_USB;

lewgReturn_t onMeeetHelp()
{
  printf("  "LEWG_STRIKE_ARG"<...>\n"
         "    tty device which controls the strike.\n"
         "    if omitted "LEWG_DEV_USB" will be used.\n\n"
         "  "LEWG_ACTION_ARG"\n"
         "    opens the door.\n\n");

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetInit()
{
  lewgReturn_t rv;
  lewgGetStrArgument(LEWG_STRIKE_ARG, &strike);

  rv = lewgGetArgument(LEWG_ACTION_ARG);
  if (rv != LEWG_SUCCESS)
  {
    onMeeetHelp();
    return LEWG_ERROR;
  }

  lewgLogInfo("Arguments  %s \n",strike);

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetStep()
{
  lewgReturn_t rv = LEWG_ERROR;;
  int fd = 0;
  
  rv = lewgStrikeInit(&fd,strike);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgStrikeOpen(fd);
  lewgStrikeDestroy(&fd);
  
  return rv;
}

