#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../io/lewg.io.ethernet.server.h"
#include "./../../utils/lewg.util.logger.h"
#include "./../../code/lewg.code.system.h"
#include "./../../code/lewg.code.localstore.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"
#include "./../../utils/lewg.util.script.h"

#ifndef LEWG_MAX_CODE_LENGTH
  #define LEWG_MAX_CODE_LENGTH 12
#endif

#ifndef LEWG_PORT
  #define LEWG_PORT 8081
#endif

#ifndef LEWG_KEY_FILE
  #define LEWG_KEY_FILE "/root/keys"
#endif

#define LEWG_USER_DIR   "./user"
#define LEWG_SYSTEM_DIR "./system"

#define LEWG_PORT_ARG      "--port="
#define LEWG_KEYFILE_ARG   "--keyfile="

#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)

static lewgSrvSocket_t sock;
static char* keyfile = LEWG_KEY_FILE;

static lewgReturn_t lewgParseCode(char* buffer, int length)
{
  lewgReturn_t rv = LEWG_ERROR;
  lewgLocalCode_t code;

  lewgLogInfo("Key %.*s \n", length, &buffer[0]);

  // Ensure the code is clean...
  rv = lewgSanatizeCode(buffer, &code);
  LEWG_ENSURE_SUCCESS_ERROR(rv,"Code not clean\n");

  // ... change to the ky file directory...
  rv = lewgChangeDirectory(LEWG_KEY_FILE);
  LEWG_ENSURE_SUCCESS_ERROR(rv,"Keyfile directory not found\n");

  // ... then switch to hte type directory...
  rv = LEWG_ERROR;
  if (code.type == '#')
    rv = lewgChangeDirectory(LEWG_USER_DIR);
  if (code.type == '.')
    rv = lewgChangeDirectory(LEWG_SYSTEM_DIR);

  LEWG_ENSURE_SUCCESS_ERROR(rv,"No user or system directory found %.*s \n", 1, &code.type);

  // ... and finally open the keyfile directory.
  rv = lewgChangeDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS_ERROR(rv,"Invalid key\n");

  // it's not an error if the initialzation fails...
  rv = lewgExecuteScript("init");
  LEWG_ENSURE_SUCCESS_ERROR(rv,"Init script failed...");

  rv = lewgValidateLifeCycle();

  if (rv == LEWG_SUCCESS)
    return lewgExecuteScript("success");

  // We can't do anything if the error script fails...
  lewgExecuteScript("error");

  return LEWG_ERROR;
}

lewgReturn_t onMeeetInit()
{
  lewgReturn_t rv = LEWG_ERROR;
  int port = LEWG_PORT;

  lewgLogInfo("Starting Server... \n");

  lewgGetStrArgument(LEWG_KEYFILE_ARG, &keyfile);
  lewgGetIntArgument(LEWG_PORT_ARG, &port);

  lewgLogInfo("Arguments  %s %d \n",keyfile,port);

  rv = lewgSrvSocketInit(&sock, port);
  LEWG_ENSURE_SUCCESS(rv);

  lewgLogInfo("Server up and listening on port %d... \n",port);
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetDestroy()
{
  lewgSrvSocketDestroy(&sock);
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetStep()
{
  int length = 5+12;
  char buffer[length];
  lewgReturn_t rv;

  rv = lewgSrvSocketAccept(&sock);

  if (rv != LEWG_SUCCESS)
    return LEWG_CONTINUE;

  rv = lewgSrvSocketRecv(&sock, &buffer[0],&length);
  lewgSrvSocketRelease(&sock);

  if (rv != LEWG_SUCCESS)
    return LEWG_CONTINUE;

  lewgLogInfo("Message recved\n");

  if ( (length < 5) ||  (memcmp(&buffer[0],"CODE\n",5) != 0) )
  {
    lewgLogInfo("Unknown Message %.*s \n", length, &buffer[0]);

    // Slow down to avoid flooding...
    return LEWG_CONTINUE;
  }

  lewgLogInfo("Code %.*s received \n",length-5,&buffer[5]);
  lewgParseCode(&buffer[5],length-5);

  return LEWG_CONTINUE;
}

lewgReturn_t onMeeetHelp()
{
  printf("  "LEWG_PORT_ARG"<...>\n"
         "    the server port\n"
         "    if omitted "STRINGIFY(LEWG_PORT)" will be used.\n\n"
         "  "LEWG_KEYFILE_ARG"<...>\n"
         "    the path to the keyfile containing the localcodes.\n"
         "    if omitted "LEWG_KEY_FILE " will be used\n\n");
         
  return LEWG_SUCCESS;
}