#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../../utils/lewg.core.types.h"
#include "../../utils/lewg.core.entrypoint.h"

#include "../../devices/lewg.device.client.h"
#include "../../devices/lewg.device.keypad.h"
#include "../../utils/lewg.util.logger.h"
#include "../../code/lewg.code.system.h"
#include "../../utils/lewg.util.system.h"

#ifndef LEWG_DEV_USB
  #define LEWG_DEV_USB "/dev/ttyUSB0"
#endif

#ifndef LEWG_PORT
  #define LEWG_PORT 3333
  //#define LEWG_PORT 8081
#endif

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)

#ifndef LEWG_IP
  #define LEWG_IP "127.0.0.1"
  //#define LEWG_IP "192.168.178.246"
#endif

#ifndef LEWG_TUNNEL_CMD
  #define LEWG_TUNNEL_CMD "dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost"
#endif


#define LEWG_IP_ARG      "--ip="
#define LEWG_KEYPAD_ARG  "--keypad="
#define LEWG_PORT_ARG    "--port="
#define LEWG_TUNNEL_ARG  "--tunnel="
#define LEWG_DIRECT_ARG  "--direct-tcp"

static int fd = 0;
static char* srvIp = LEWG_IP;
static int srvPort = LEWG_PORT;
static char* tunnel = NULL;


lewgReturn_t onMeeetHelp()
{
  printf("  "LEWG_KEYPAD_ARG"<...>\n"
         "    defines which tty should be used as keypad input.\n"
         "    if omitted "LEWG_DEV_USB" will be used.\n \n"
         "  "LEWG_IP_ARG"<xxx.xxx.xxx.xxx>\n"
         "    sets the server's ip address.\n"
         "    if omitted "LEWG_IP" is used\n\n"
         "  "LEWG_PORT_ARG"<...>\n"
         "    sets the server's port\n"
         "    if omitted "STRINGIFY(LEWG_PORT)" is used\n\n"
         "  "LEWG_TUNNEL_ARG"<...>\n"
         "    the command which is executed to establish the tunnle.\n"
         "    all environment variables looped to the command\n"
         "    if ommitted \""LEWG_TUNNEL_CMD"\" is used\n\n"
         "  "LEWG_DIRECT_ARG"\n"
         "    a direct connection will be used instead of a secure tunnel\n\n");

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetInit()
{
  lewgReturn_t rv = LEWG_ERROR;
  char* device = LEWG_DEV_USB;

  lewgGetStrArgument(LEWG_KEYPAD_ARG, &device);
  lewgGetStrArgument(LEWG_IP_ARG,&srvIp);
  lewgGetIntArgument(LEWG_PORT_ARG, &srvPort);

  rv = lewgGetArgument(LEWG_DIRECT_ARG);
  if (rv == LEWG_ERROR)
  {
    tunnel = LEWG_TUNNEL_CMD;
    lewgGetStrArgument(LEWG_TUNNEL_ARG, &tunnel);
  }

  if (srvPort == 0)
  {
    lewgLogError("Invalid port\n");
    return LEWG_ERROR;
  }

  rv = lewgKeyPadInit(&fd, device);

  LEWG_ENSURE_SUCCESS_ERROR(rv,"No keypad found\n");
  //if (rv != LEWG_SUCCESS)
  //{
  //  lewgLogError("No keypad found\n");
  //  return rv;
  //}

  lewgLogInfo("Arguments %s %s %d \n",device,srvIp,srvPort);
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetDestroy()
{
  lewgKeyPadDestroy(&fd);

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetStep()
{
  lewgReturn_t rv = LEWG_ERROR;

  const int BUFFER_LENGTH = 12;
  char buffer[BUFFER_LENGTH];

  rv = lewgKeyPadGetCode(&fd, &buffer[0], BUFFER_LENGTH);
  if (rv != LEWG_SUCCESS)
  {
    do
    {
      lewgLogError("Connection to keypad lost reconnecting in 10 seconds...\n");
      onMeeetDestroy();

      sleep(10);
      rv = onMeeetInit();
      
    } while (rv == LEWG_ERROR);

    lewgLogInfo("Succesfully reconnected to keypad...\n");
    return LEWG_CONTINUE;
  }

  lewgLogInfo("Transmitting Code... \n");
  lewgClientSend(srvIp, srvPort, tunnel, &buffer[0], BUFFER_LENGTH);

  rv = lewgIsResetCode(&buffer[0],BUFFER_LENGTH);
  if ( rv == LEWG_SUCCESS)
  {
    lewgReboot();
    return LEWG_SUCCESS;
  }

  rv = lewgIsShutdownCode(&buffer[0], BUFFER_LENGTH);
  if ( rv == LEWG_SUCCESS)
  {
    lewgShutdown();
    return LEWG_SUCCESS;
  }

  return LEWG_CONTINUE;
}

