
#ifndef LEWG_CLIENT_SOCKET_H_
  #define LEWG_CLIENT_SOCKET_H_

  #include <stdio.h>
  #include <sys/socket.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <string.h>
  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"
  #include "./../io/lewg.io.tunnel.h"
  
  typedef struct {
    int socket;
    lewgTunnel_t tunnel;
  } lewgSocket_t;

  lewgReturn_t lewgSocketInit(lewgSocket_t *socket, const char* ip, int port);
  lewgReturn_t lewgSocketInitEx(lewgSocket_t* sock, const char* ip, int port,
                                  char* cmd, char* env);
  lewgReturn_t lewgSocketDestroy(lewgSocket_t* socket);
  lewgReturn_t lewgSocketSend(lewgSocket_t* socket, const char* bytes, int length);
  lewgReturn_t lewgSocketRecv(lewgSocket_t* socket, char* bytes, int* length);

#endif /* LEWG_CLIENT_ETHERNET_H_ */
