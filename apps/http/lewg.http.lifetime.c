#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../utils/lewg.util.logger.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_UNISTD_H
  #include <unistd.h>
#endif


#include <stdlib.h>

extern char **environ;

static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    
    for ( ; *envp != NULL; envp++) 
      printf("%s\n", *envp);
      
    printf("</pre><p>\n");
}

lewgReturn_t onMeeetHelp()
{
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetInit()
{
  return LEWG_SUCCESS;
}

lewgReturn_t onMeeetStep()
{
  char* code = "123456";
  
  // step 1: sanatize code...
  
  // step 2: change directory...
  rv = lewgChangeDirectory("/srv/keys/user/");
  LEWG_ENSURE_SUCCESS_ERROR(rv, "Key directory not found\n");

  rv = lewgChangeDirectory(&code[0]);
  LEWG_ENSURE_SUCCESS_ERROR(rv, "Invalid key")

  rv = lewgFileOpen(&fh, "lifecycle");
  LEWG_ENSURE_SUCCESS(rv);
  
  while (lewgFileReadln(&fh, &buffer[0], LENGTH) == LEWG_SUCCESS)
  {
    rv = lewgIsComment(&buffer[0]);
    if (rv == LEWG_SUCCESS)
      continue;

    rv = lewgIsWhiteSpace(&buffer[0]);
    if (rv == LEWG_SUCCESS)
      continue;
      
   printf("%s\r\n",&buffer[0]);
  }
  
  lewgFileClose(&fh);
  
  DIR* dir = opendir("/srv/keys/user/");

  if (dir == 0)
  {
    if (errno != 0)
      printf("HTTP/1.1 500 %s\r\n\r\n%s\r\n",strerror(errno),strerror(errno));
    else
      printf("HTTP/1.1 500\r\n\r\nUnknown error\r\n");

    printf("\r\n");
    return LEWG_SUCCESS;
  }

  printf("HTTP/1.1 200 OK\r\n");
  printf("Content-Type: text/plain\r\n" );
  printf("\r\n");

  struct dirent* next = 0;
  while ((next = readdir(dir)) != 0)
  {

    if (next->d_type != DT_DIR)
      continue;

    // check if it's a valid code file...
    printf("%s\n",&next->d_name[0]);
  }

  closedir(dir);

  // Get char waits blocking...
  //char buf = getchar();
  //lewgLogInfo("Sending byte %c \n", buf);

  char *contentLength = getenv("CONTENT_LENGTH");
  int len = 0;

  if (contentLength != NULL)
    len = strtol(contentLength, NULL, 10);

  if (len <= 0)
    printf("No data from standard input.<p>\n");
  else {
    int i, ch;

    for (i = 0; i < len; i++) {
      if ((ch = getchar()) < 0) {
        printf("Error: Not enough bytes received on standard input<p>\n");
        break;
      }
  
      putchar(ch);
    }
  } 
  PrintEnv("Request environment", environ);

  return LEWG_SUCCESS;
}



lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}


