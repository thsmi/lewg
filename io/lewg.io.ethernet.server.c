#include "lewg.io.ethernet.server.h"

lewgReturn_t lewgSrvSocketInit(lewgSrvSocket_t* sock, int port)
{

  int rv = 0;
  int srvSock = -1;

  srvSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (srvSock < 0) {
    lewgLogError("Creating server socket failed\n");
    return LEWG_ERROR;
  }

  sock->server = srvSock;
  sock->client = -1;

  struct sockaddr_in srvAddr;
  memset(&srvAddr,0,sizeof(srvAddr));

  srvAddr.sin_family = AF_INET;
  srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  srvAddr.sin_port = htons(port);

  rv = bind(sock->server, (struct sockaddr*) &srvAddr, sizeof(srvAddr));

  if (rv <0) {
    lewgLogError("Binding Server socket failed\n");
    lewgSrvSocketDestroy(sock);

    return LEWG_ERROR;
  }

  rv = listen(sock->server, 10);

  if (rv <0) {
    lewgLogError("Putting server socket into listening mode failed\n");
    lewgSrvSocketDestroy(sock);

    return LEWG_ERROR;
  }

  // Switch to non blocking sockets
  // fcntl(sock->server, F_SETFL, O_NONBLOCK);

  lewgLogInfo("Server listening on port %d\n",port);
  return LEWG_SUCCESS;
}

lewgReturn_t lewgSrvSocketDestroy(lewgSrvSocket_t* sock)
{
  if (sock == 0 || sock->server == -1)
    return LEWG_SUCCESS;

  // release any client connections...
  if (sock->client != -1) {
    lewgSrvSocketRelease(sock);
    sock->client = -1;
  }

  close(sock->server);
  sock->server = -1;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSrvSocketAccept(lewgSrvSocket_t* sock)
{
  int cltSock;
  struct sockaddr_in cltAddr;
  unsigned int cltAddrLen = sizeof(cltAddr);


  cltSock = accept(sock->server, (struct sockaddr*) &cltAddr, &cltAddrLen);

  if (cltSock <0) {
    // Continue in case the socket would block...
    //if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    //  return LEWG_CONTINUE;

    lewgLogError("Accepting incoming connection failed\n");
    return LEWG_ERROR;
  }

  sock->client = cltSock;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSrvSocketRelease(lewgSrvSocket_t* sock)
{
  if (sock == 0 || sock->client == -1)
    return LEWG_SUCCESS;

  close(sock->client);
  sock->client = -1;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSrvSocketSend(lewgSrvSocket_t* sock, const char* bytes, int length)
{
  int rv = 0;
  rv = send(sock->client,bytes,length,0);
  
  if (rv != length) {
    // a transmission error is no need to destroy the socket.
    lewgLogError("Sending telegram failed\n");
    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}


lewgReturn_t lewgSrvSocketRecv(lewgSrvSocket_t* sock, char* buffer, int* length)
{
  int rv = 0;  
  
  if (sock->client == -1)
  {
    lewgLogError("Client socket not initialized\n");
    sleep(1);
    
    return LEWG_ERROR;
  }
  
  rv = recv(sock->client,buffer,*length,MSG_WAITALL);
  
  if (rv < 0)
  {
    //if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
    //  return LEWG_CONTINUE;

    lewgLogInfo("Socket receive error %s \n",strerror(errno));

    sleep(1);
    return LEWG_ERROR;
  }

  if (rv != *length ) {
    lewgLogError("Expected %d bytes but received %d %.*s\n", *length, rv, rv, &buffer[0]);

    *length = 0;
    return LEWG_ERROR;
  }

  *length = rv;

  return LEWG_SUCCESS;
}

