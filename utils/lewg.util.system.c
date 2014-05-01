
#include "lewg.util.system.h"

lewgReturn_t lewgSystemCall(const char* command)
{
  int rv = -1;

  lewgLogInfo("Executing System command %s \n", command);

  #ifdef LEWG_EMULATE_SYSTEM_CALL
    rv = 0;
  #else
    rv = system(command);
  #endif
 
  if (rv == -1)
  {
    lewgLogError("System call failed %s \n", command);
    return LEWG_ERROR;
  }
  
  return LEWG_SUCCESS;
}

lewgReturn_t lewgReboot()
{
  return lewgSystemCall("reboot");
}

lewgReturn_t lewgShutdown()
{
  return lewgSystemCall("shutdown");
}
