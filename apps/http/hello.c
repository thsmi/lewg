#include "stdio.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>


static void listKeys()
{
  DIR* dir = opendir("/srv/keys/");
  //DIR* dir = opendir("/root/keys/");

  if (dir == 0)
  {
    printf("{\n");

    if (errno != 0)
      printf("  \"message\":\"%s\", \n",strerror(errno));

    printf("  \"error\":\"true\" \n"); 
    printf("}\n");
    return;
  }
  
  printf("{\n");
  printf("  \"keys\": [\n");
  
  struct dirent* next = 0;

  while ((next = readdir(dir)) != 0)
  {

    if ((next->d_type != DT_REG) && (next->d_type != DT_LNK))
      continue;

   // check if it's a valid code file...
    printf("    {\n");
    printf("      \"code\":\"%s\",\n",&next->d_name[0]);
    printf("      \"blubb\":\"test\"\n");
    printf("    }\n");
  }

  printf("  ]\n");
  printf("}\n");
  closedir(dir);
}

static void listKey()
{

[
  {
    "timestamp" : "";
    "comment" : "";
  }

//{
//  [
//    {"active": "true"},
//    {}
//  ]
//}
//  Comment

//  Active
//  Active

//  Command
}


int main(void)
{
  printf( "Content-Type: text/plain\n\n" );

  listKeys();

  return 0;
  }