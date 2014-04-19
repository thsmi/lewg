#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../utils/lewg.util.logger.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"
#include "./../../io/lewg.io.stdio.h"
#include "./../../io/lewg.io.file.h"
#include "./../../io/lewg.io.directory.h"
#include "./../../code/lewg.code.localstore.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_UNISTD_H
  #include <unistd.h>
#endif


#include <stdlib.h>

//extern char **environ;

//static void PrintEnv(char *label, char **envp)
//{
//    printf("%s:<br>\n<pre>\n", label);
//    
//    for ( ; *envp != NULL; envp++) 
//      printf("%s\n", *envp);
//      
//    printf("</pre><p>\n");
//}

lewgReturn_t onMeeetInit()
{
  return LEWG_SUCCESS;
}

// & = separator
// + = space
//%0x%x to ascii

static lewgReturn_t lewgNewKey(char* buffer)
{

  lewgReturn_t rv;
  lewgLocalCode_t code;
  lewgFileHandle_t fh;

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS_ERROR(rv, "Code not clean\n");

  lewgLogInfo("Create key %s\n",&code.code[0]);

  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgCreateDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgChangeDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  symlink("./../.template/init","./init");
  symlink("./../.template/error","./error");
  symlink("./../.template/success","./success");

  // create an empty keyfile...
  rv = lewgOpenFile(&fh,"lifecycle","w+");
  LEWG_ENSURE_SUCCESS(rv);

  lewgCloseFile(&fh);

  return LEWG_SUCCESS;
}

static lewgReturn_t lewgDeleteKey(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);

  lewgLogInfo("Delete key %s\n", &code.code[0]);

  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgDeleteDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  return LEWG_SUCCESS;
}


static lewgReturn_t lewgPrintLifeCycle(char* code)
{
  lewgReturn_t rv;
  char buf[256];

  printf("LIFECYCLE %s\n",code);

  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgChangeDirectory(&code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  lewgFileHandle_t fh;

  rv = lewgOpenFile(&fh,"lifecycle","r");
  LEWG_ENSURE_SUCCESS(rv);

  while(lewgFileReadln(&fh, &buf[0], 256) == LEWG_SUCCESS)
    printf("  %s",&buf[0]);

  lewgCloseFile(&fh);
  
  return LEWG_SUCCESS;
}

static lewgReturn_t lewgPrintComment(char* code)
{
  lewgReturn_t rv;
  char buf[1024];

  printf("COMMENT %s\n",code);

  rv = lewgChangeDirectory("/srv/keys/user");
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgChangeDirectory(&code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  lewgFileHandle_t fh;

  rv = lewgOpenFile(&fh, "comment","r");
  LEWG_ENSURE_SUCCESS(rv);

  while(lewgFileReadln(&fh, &buf[0], 1024) == LEWG_SUCCESS)
    printf("  %s",&buf[0]);

  lewgCloseFile(&fh);

  return LEWG_SUCCESS;
}

static lewgReturn_t lewgGetLifeCycle(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgPrintLifeCycle(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  return LEWG_SUCCESS;
}

static lewgReturn_t lewgListKeys(void)
{
  lewgReturn_t rv;
  lewgDirectoryHandle_t dh;
  lewgDirectoryItem_t item;

  rv = lewgOpenDirectory(&dh,"/srv/keys/user");
  LEWG_ENSURE_SUCCESS(rv);

  while (lewgNextDirectory(&dh,&item) == LEWG_SUCCESS)
  {
    rv = lewgIsDirectory(&item);
    if (rv != LEWG_SUCCESS)
      continue;

    if (memcmp(&item.name[0],".\0",1) == 0)
      continue;

    if (memcmp(&item.name[0],"..\0",2) == 0)
      continue;

    lewgPrintLifeCycle(&item.name[0]);
    lewgStdOutPrint("\n\n");
    lewgPrintComment(&item.name[0]);
    lewgStdOutPrint("\n\n");
  }

  lewgCloseDirectory(&dh);

  return LEWG_SUCCESS;
}

static lewgReturn_t lewgUpdateLifeCycle(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;
  char buf[256];

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);
  
  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS(rv);
  
  rv = lewgChangeDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);
  
  lewgFileHandle_t fh;
  rv = lewgOpenFile(&fh, "lifecycle2","w+");
  LEWG_ENSURE_SUCCESS(rv);

  time_t begin, end;
  while (lewgStdInReadln(&buf[0], 1024) == LEWG_SUCCESS)
  {
    if (memcmp("  ",&buf[0], 2) != 0)
      continue;

    // Check if the interval can be parsed...
    rv = lewgParseInterval(&buf[2], &begin, &end);
    if (rv == LEWG_ERROR)
      continue;

    lewgFilePrint(&fh, "%s", &buf[2]);
  }

  lewgCloseFile(&fh);

  // create temporary file
  // read from std in..

  // rename lifecycle
  // delete file


  //parse std in lines and replace existing lifecyle file
  return LEWG_SUCCESS;
}

static lewgReturn_t lewgGetComment(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;
  //uint8_t buf[256];

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgPrintComment(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  return LEWG_SUCCESS;
}


static lewgReturn_t lewgUpdateComment(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;
  char buf[1024];

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgChangeDirectory(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  lewgFileHandle_t fh;

  rv = lewgOpenFile(&fh, "comment","w+");
  LEWG_ENSURE_SUCCESS(rv);

  while (lewgStdInReadln(&buf[0], 1024) == LEWG_SUCCESS)
  {
    if (memcmp("  ",&buf[0], 2) != 0)
      continue;

    lewgFilePrint(&fh, "%s", &buf[2]);
  }

  lewgCloseFile(&fh);

  //open key directory
  //parse std in lines and replace existing lifecyle file
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetService()
{
  lewgReturn_t rv;
  
  lewgLogInfo("Blubber\n");

  lewgStdOutPrint("HTTP/1.1 200 OK\r\n");
  lewgStdOutPrint("Content-Type: text/plain\r\n" );
  lewgStdOutPrint("\r\n");

//  if getenv("REQUEST_METHOD") not equals "POST"
//    return ERROR;
  char buffer[80];

  rv = lewgStdInReadln(&buffer[0], 80);
  LEWG_ENSURE_SUCCESS(rv);


  if (memcmp(&buffer[0],"LIST\n", 5) == 0)
    return lewgListKeys();

  if (memcmp(&buffer[0],"NEW ", 4) == 0)
    return lewgNewKey(&buffer[4]);

  if (memcmp(&buffer[0],"DELETE ",7) == 0)
    return lewgDeleteKey(&buffer[7]);

  if (memcmp(&buffer[0],"GET LIFECYCLE ", 14) == 0)
    return lewgGetLifeCycle(&buffer[14]);

  if (memcmp(&buffer[0],"UPDATE LIFECYCLE ", 17) == 0)
    return lewgUpdateLifeCycle(&buffer[17]);

  if (memcmp(&buffer[0],"GET COMMENT ", 12) == 0)
    return lewgGetComment(&buffer[12]);

  if (memcmp(&buffer[0],"UPDATE COMMENT ", 15) == 0)
    return lewgUpdateComment(&buffer[15]);

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}
