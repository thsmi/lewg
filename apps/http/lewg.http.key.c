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
    lewgStdOutPrint("  %s",&buf[0]);

  lewgCloseFile(&fh);

  lewgStdOutPrint("\n");

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
    lewgStdOutPrint("  %s",&buf[0]);

  lewgCloseFile(&fh);

  lewgStdOutPrint("\n");

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

static lewgReturn_t lewgGetComment(char* buffer)
{
  lewgReturn_t rv;
  lewgLocalCode_t code;
  //uint8_t buf[256];
  
  lewgLogInfo("Get Comment Called\n");

  rv = lewgSanatizeCode(&buffer[0],&code);
  LEWG_ENSURE_SUCCESS(rv);

  rv = lewgPrintComment(&code.code[0]);
  LEWG_ENSURE_SUCCESS(rv);

  return LEWG_SUCCESS;
}


static lewgReturn_t lewgSetLifeCycle(char* buffer)
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

  char tmpname[] = "lifecycle.XXXXXX";
  lewgFileHandle_t fh;

  rv = lewgCreateTempFile(&fh,tmpname,"w+");
  LEWG_ENSURE_SUCCESS(rv);
  
  time_t begin, end;
  while (lewgStdInReadln(&buf[0], 1024) == LEWG_SUCCESS)
  {
    // an unindented new line ends our protocol regulary...
    if (memcmp("\n",&buf[0], 1) == 0)
      break;

    // ... everything else has to be indented.
    if (memcmp("  ",&buf[0], 2) != 0)
      break;

    // ... we also drop empty statements...
    if (memcmp("  \n",&buf[0], 3) == 0)
      continue;

    // Check if the interval can be parsed...
    rv = lewgParseInterval(&buf[2], &begin, &end);
    if (rv == LEWG_ERROR)
    {
      lewgLogInfo("Incompatilbe Timestamp %s",&buf[2]);
      continue;
    }

    lewgFilePrint(&fh, "%s", &buf[2]);
  }

  lewgCloseFile(&fh);

  lewgRenameFile(tmpname, "lifecycle");
  
  lewgLogInfo("Lifecycle for %s successfully updated \n", &code.code[0]);

  return LEWG_SUCCESS;
}

static lewgReturn_t lewgSetComment(char* buffer)
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

  char tmpname[] = "comment.XXXXXX";
  lewgFileHandle_t fh;

  rv = lewgCreateTempFile(&fh,tmpname,"w+");
  LEWG_ENSURE_SUCCESS(rv);

  while (lewgStdInReadln(&buf[0], 1024) == LEWG_SUCCESS)
  {
    // an unindented new line ends our protocol regulary...
    if (memcmp("\n",&buf[0], 1) == 0)
      break;

    // ... everything else has to be indented.
    if (memcmp("  ",&buf[0], 2) != 0)
      break;

    lewgFilePrint(&fh, "%s", &buf[2]);
  }

  lewgCloseFile(&fh);

  lewgRenameFile(tmpname, "comment");

  lewgLogInfo("Comment for %s successfully updated \n", &code.code[0]);

  return LEWG_SUCCESS;
}


static lewgReturn_t lewgCreateCookie()
{

}

static lewgReturn_t lewgGetCookie()
{
}

static lewgReturn_t lewgDeleteCookie()
{
}


static lewgReturn_t lwegCreateSession()
{

}

static lewgReturn_t lewgGetSession()
{

}

static lewgReturn_t lewgDeleteSession()
{

}

lewgReturn_t onMeeetService()
{
  lewgReturn_t rv;
  char buf[1024];

  lewgStdOutPrint("HTTP/1.1 200 OK\r\n");
  lewgStdOutPrint("Content-Type: text/plain\r\n" );
  lewgStdOutPrint("\r\n");

//  if getenv("REQUEST_METHOD") not equals "POST"
//    return ERROR;

  // Commands may be transmitted in a batch...
  while (lewgStdInReadln(&buf[0], 1024) == LEWG_SUCCESS)
  {
    // We ignore white spaces...
    if (lewgIsWhiteSpace(&buf[0]) == LEWG_SUCCESS)
      continue;

    if (memcmp("LIST\n", &buf[0], 5) == 0)
      rv = lewgListKeys();
    else if (memcmp("NEW ", &buf[0], 4) == 0)
      rv = lewgNewKey(&buf[4]);
    else if (memcmp("DELETE ", &buf[0], 7) == 0)
      rv = lewgDeleteKey(&buf[7]);
    else if (memcmp("SET LIFECYCLE ",&buf[0],14) == 0)
      rv = lewgSetLifeCycle(&buf[14]);
    else if (memcmp("GET LIFECYCLE ", &buf[0], 14) == 0)
      rv = lewgGetLifeCycle(&buf[14]);
    else if (memcmp("SET COMMENT ", &buf[0],12) == 0)
      rv = lewgSetComment(&buf[12]);
    else if (memcmp("GET COMMENT ", &buf[0] , 12) == 0)
      rv = lewgGetComment(&buf[12]);
    else
    {
      lewgLogInfo("Unknown command %s",&buf[0]);
      return LEWG_ERROR;
    }

    if (rv != LEWG_SUCCESS)
      lewgLogInfo("Die with error %s",&buf[0]);

    LEWG_ENSURE_SUCCESS(rv);
  }

  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}
