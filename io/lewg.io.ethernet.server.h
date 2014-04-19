
#ifndef  LEWG_IO_ETHERNET_SERVER_H_
  #define LEWG_IO_ETHERNET_SERVER_H_

  #include <stdio.h>
  #include <sys/socket.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <string.h>
  #include <errno.h>
  #include <fcntl.h>

  #include "../utils/lewg.core.types.h"
  #include "../utils/lewg.util.logger.h"

  typedef struct {
    int server;
    int client;
  } lewgSrvSocket_t;

  lewgReturn_t lewgSrvSocketInit(lewgSrvSocket_t* sock, int port);
  lewgReturn_t lewgSrvSocketDestroy(lewgSrvSocket_t* sock);

  lewgReturn_t lewgSrvSocketAccept(lewgSrvSocket_t* sock);
  lewgReturn_t lewgSrvSocketRelease(lewgSrvSocket_t* sock);

  lewgReturn_t lewgSrvSocketRecv(lewgSrvSocket_t* sock, char* buffer, int* length);
  lewgReturn_t lewgSrvSocketSend(lewgSrvSocket_t* sock, const char* bytes, int length);

#endif /* LEWG_ETHERNET_H_ */
