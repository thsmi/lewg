#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../../utils/lewg.util.logger.h"
#include "./../../utils/lewg.core.types.h"
#include "./../../utils/lewg.core.args.h"
#include "./../../io/lewg.io.stdin.h"

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

lewgReturn_t onMeeetInit()
{
  return LEWG_SUCCESS;
}

// & = separator
// + = space
//%0x%x to ascii


lewgReturn_t onMeeetService()
{
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

//    while ( nextParameter(&name, &buffer) == LEWG_SUCCESS)
//    {
//      if (strcmp(name,"code"))
//      {
//        getParameter(&code,&buffer);
//      }
//    }

//    lewgSanatizeCode();
    
      char buffer[80];
      
      lewgStdInReadln(&buffer[0], 80);
      printf("\n\nPayload\n%s",&buffer[0]);
//    lewgFileHandle_t* fh;
    
//    lewgOpenStdIn(&fh);
    
//    rv = lewgFileReadln(&fh, &buffer[0], 80);
//    LEWG_ENSURE_SUCCESS(rv)
    
    // Test if string equals list
    // Test if string equals list *....#
    // Test if string equals delete *....#
    // Test if string equals new *...#

      
//    }
    
//    while (fgets(buff, sizeof(buff), stdin) != null);
    
//        int i, ch;

//    for (i = 0; i < len; i++) {
//      if ((ch = getchar()) < 0) {
//        printf("Error: Not enough bytes received on standard input<p>\n");
//        break;
//      }
  
//      putchar(ch);
//    }
  } 
  PrintEnv("\n\nRequest environment", environ);

  return LEWG_SUCCESS;
}



lewgReturn_t onMeeetDestroy()
{
  return LEWG_SUCCESS;
}


