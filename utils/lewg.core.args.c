
#include "./../utils/lewg.core.args.h"

static int lewgArgc = 0;
static char** lewgArgv = 0;

lewgReturn_t lewgInitArguments(int argc, char **argv)
{
  lewgArgc = argc;
  lewgArgv = argv;

  atexit(lewgDestroyArguments);

  return LEWG_SUCCESS;
}

void lewgDestroyArguments()
{
  lewgArgc = 0;
  lewgArgv = 0;
}

lewgReturn_t lewgGetArgument(const char* option)
{
  return lewgGetStrArgument(option,0);
}

lewgReturn_t lewgGetStrArgument(const char* option, char** arg)
{
  int i;

  for (i=0; i<lewgArgc; i++)
  {
    int len = strlen(option);
    if (memcmp(option,&(lewgArgv[i][0]),len) != 0)
      continue;

    
    if (arg != 0)
      *arg = &(lewgArgv[i][len]);

    return LEWG_SUCCESS;
  }

  return LEWG_ERROR;
}

lewgReturn_t lewgGetIntArgument(const char* option, int* arg)
{
  char* tmp = 0;
  lewgReturn_t rv;
  
  rv = lewgGetStrArgument(option, &tmp);
  
  LEWG_ENSURE_SUCCESS_INFO(rv,"No argument %s found\n",option);
  
//  if (rv != LEWG_SUCCESS)
//  {
//    lewgLogInfo("No argument %s found\n",option);
//    return rv;
//  }
    
  *arg = strtol(tmp,0,10);
  
  return LEWG_SUCCESS;
}