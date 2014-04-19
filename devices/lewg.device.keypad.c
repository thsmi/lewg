
#include "lewg.device.keypad.h"

#define BUFFER_LENGTH 12

lewgReturn_t lewgKeyPadInit(int* fd, const char* device)
{
  #ifndef LEWG_EMULATE_KEYPAD
    return lewgInitPort(fd,device);
  #else
    return LEWG_SUCCESS;
  #endif
}

lewgReturn_t lewgKeyPadGetCode(int* fd, char* buffer, int buflen)
{
  int pos = 0;

  while (1)
  {

    // Ensure we are not running out of bounds...
    if (pos > buflen)
    {
      pos = 0;
      memset(buffer,0,buflen);
    }

    #ifndef LEWG_EMULATE_KEYPAD
      lewgReturn_t rv = lewgReadChar(*fd, &buffer[pos]);

      // No new bytes? Would block? 
      if (rv == LEWG_CONTINUE)
      {
        usleep(200000);
        continue;
      }

      // any errors?
      if (rv == LEWG_ERROR)
        return rv;
    #else
      buffer[pos] = getchar();
      fflush(stdin);
    #endif

    switch (buffer[pos])
    {
      case '*' :
        lewgLogInfo("Reset Character detected, expunging buffers... \n");

        memset(buffer,0,buflen);

        buffer[0] = '*';
        pos=1;

        continue;

      case '0': case '1':
      case '2': case '3':
      case '4': case '5':
      case '6': case '7':
      case '8': case '9':
      case 'A': case 'B':
      case 'C':
        lewgLogInfo("Character %c detected... \n",buffer[pos]);

        pos += 1;
        break;

      case '#' :
      case '.' :
        lewgLogInfo("End Char detected... \n",buffer[pos]);
        
        return LEWG_SUCCESS;

      default :
        usleep(200000);
    }
  }

  return LEWG_ERROR;
}

lewgReturn_t lewgKeyPadDestroy(int* fd)
{
  #ifndef LEWG_EMULATE_KEYPAD
    return lewgClosePort(fd);
  #else
    return LEWG_SUCCESS;
  #endif
}
