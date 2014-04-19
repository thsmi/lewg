#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../utils/lewg.util.logger.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"
#include "./../../io/lewg.io.serial.h"

#ifndef LEWG_DEV_USB
  #define LEWG_DEV_USB "/dev/ttyVirtualS1"
#endif

#define LEWG_KEYPAD_ARG  "--keypad="

static int fd = 0;
static char* device = LEWG_DEV_USB;

lewgReturn_t onMeeetHelp()
{
  printf("  "LEWG_KEYPAD_ARG"<...>\n"
         "    defines which tty should be used as keypad input.\n"
         "    if omitted "LEWG_DEV_USB" will be used.\n \n");

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetInit()
{
  lewgGetStrArgument("--keypad=", &device);
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetStep()
{
  lewgReturn_t rv;

  rv = lewgInitPort(&fd,device);
  if (rv != LEWG_SUCCESS)
    return LEWG_CONTINUE;

  // Get char waits blocking...
  char buf = getchar();
  lewgLogInfo("Sending byte %c \n", buf);

  lewgWriteChar(fd,&buf);

   lewgClosePort(&fd);
  return LEWG_CONTINUE;
}

lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}


