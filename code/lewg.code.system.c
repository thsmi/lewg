
#include "lewg.code.system.h"

static const int MAX_CODE_LEN = 12;

lewgReturn_t  lewgIsResetCode(const char* buffer, int length)
{
  // Reset codes are exactly 12 characters long...
  if (length < MAX_CODE_LEN)
    return LEWG_ERROR;

  if (memcmp(buffer,"*6524834035.", MAX_CODE_LEN) != 0)
    return LEWG_ERROR;

  return LEWG_SUCCESS;
}


lewgReturn_t  lewgIsShutdownCode(const char* buffer, int length)
{
  // Shutdown codes are exactly 12 character long..
  if (length < MAX_CODE_LEN)
    return LEWG_ERROR;

  if (memcmp(buffer,"*9453803651.", MAX_CODE_LEN) != 0)
    return LEWG_ERROR;

  return LEWG_SUCCESS;
}
