#include "lewg.device.strike.h"

lewgReturn_t lewgStrikeInit(int* fd, const char* device)
{
  #ifndef LEWG_EMULATE_STRIKE
    return lewgInitPort(fd,device);
  #else
    return LEWG_SUCCESS;
  #endif
}

lewgReturn_t lewgStrikeOpen(int fd)
{
  lewgLogInfo("Opening door, sending magic char \n");

  #ifndef LEWG_EMULATE_STRIKE
    return lewgWriteChar(fd,"q");
  #else
    return LEWG_SUCCESS;
  #endif
}

lewgReturn_t lewgStrikeDestroy(int* fd)
{
  #ifndef LEWG_EMULATE_STRIKE
    return lewgClosePort(fd);
  #else
    return LEWG_SUCCESS;
  #endif
}