#include "lewg.util.script.h"

lewgReturn_t lewgExecuteScript(const char* file)
{
  const int LENGTH = 1024;
  char buffer[LENGTH];

  lewgReturn_t rv = LEWG_SUCCESS;
  lewgFileHandle_t fh;

  rv = lewgOpenFile(&fh, file,"r");
  LEWG_ENSURE_SUCCESS(rv);

  while (lewgFileReadln(&fh, &buffer[0], LENGTH) == LEWG_SUCCESS)
  {
    lewgLogInfo("Parsing Line %s\n",&buffer[0]);

    rv = lewgIsComment(&buffer[0]);
    if (rv == LEWG_SUCCESS)
      continue;

    rv = lewgIsWhiteSpace(&buffer[0]);
    if (rv == LEWG_SUCCESS)
       continue;

    lewgLogInfo("Executing System Call\n");

    rv = lewgSystemCall(&buffer[0]);
    if (rv == LEWG_SUCCESS)
      continue;

    lewgLogInfo("System call failed");
    lewgCloseFile(&fh);
    return LEWG_ERROR;
  }

  lewgCloseFile(&fh);
  return LEWG_SUCCESS;
}

