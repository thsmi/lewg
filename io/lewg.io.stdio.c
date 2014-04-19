#include "lewg.io.stdio.h"

lewgReturn_t lewgStdInReadln(char* buffer, const int len)
{
  char* rv = 0;
  rv = fgets(&buffer[0], len, stdin);

  // on success fgets return a pointer to our buffer
  // in case of an error or the eof was reached, null is returned
  if (rv == 0)
    return LEWG_ERROR;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgStdOutPrint(char* format, ...)
{
  va_list args;
  va_start(args, format);

  vfprintf(stdout, format, args);

  va_end(args);

  return LEWG_SUCCESS;
}