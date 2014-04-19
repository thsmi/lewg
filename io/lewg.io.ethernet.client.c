
#include "lewg.io.ethernet.client.h"

// Socket either successd and returns a socket. Or it fails and does not return a socket
// In case the initialization failed the socket is automatically destroyed, so you man
// not call SocketDestroy();

lewgReturn_t lewgSocketInitEx(lewgSocket_t* sock, const char* ip, int port, 
                                char* cmd, char* env)
{
  lewgReturn_t rv = LEWG_ERROR;
  sock->socket = 0;

  rv = lewgInitTunnel(&(sock->tunnel),cmd,env);

  if (rv != LEWG_SUCCESS)
  {
    lewgLogError("Creating tunnel failed");
    lewgSocketDestroy(sock);

    return rv;
  }

  sock->socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sock->socket < 0)
  {
    lewgLogError("Creating socket failed");
    lewgSocketDestroy(sock);

    return LEWG_ERROR;
  }

  struct sockaddr_in sockaddr;

  memset(&sockaddr,0, sizeof(sockaddr));

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ip);
  sockaddr.sin_port = htons(port);

  int ret = 0;
  ret = connect(sock->socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

  if (ret <0)
  {
    lewgLogError("Remote Socket not reachable\n");
    lewgSocketDestroy(sock);

    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSocketInit(lewgSocket_t* sock, const char* ip, int port)
{
  return lewgSocketInitEx(sock, ip, port, 0, 0);
}

lewgReturn_t lewgSocketDestroy(lewgSocket_t* sock)
{
  if (sock == 0)
    return LEWG_SUCCESS;

  lewgDestroyTunnel(&(sock->tunnel));

  if (sock->socket == 0)
    return LEWG_SUCCESS;

  close(sock->socket);
  sock->socket = 0;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSocketSend(lewgSocket_t* sock, const char* bytes, int length)
{

 
  int rv = 0;
  rv = send(sock->socket,bytes,length,0);

  if (rv != length)
  {
    // a transmission error is no need to destroy the socket.
    lewgLogError("Sending telegram failed");
    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSocketRecv(lewgSocket_t* sock, char* bytes, int* length)
{
  if ((sock == 0) || (sock->socket == 0))
  {
    lewgLogError("Socket not initialized");
    return LEWG_ERROR;
  }

  int rv = 0;
  rv = recv(sock->socket,bytes,*length,0);

  if (rv != *length)
  {
    lewgLogError("Expected %d bytes but received %d \n", *length, rv);
    *length = 0;

    return LEWG_ERROR;
  }

  *length = rv;
  
  return LEWG_SUCCESS;
}


/*lewgReturn_t lewgSocketInit(int* sock, const char* ip, int port)
{
  int rv = 0;

  *sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (*sock < 0)
  {
    lewgLogError("Creating socket failed");
    lewgSocketDestroy(sock);

    return LEWG_ERROR;
  }

  struct sockaddr_in sockaddr;

  memset(&sockaddr,0, sizeof(sockaddr));

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr(ip);
  sockaddr.sin_port = htons(port);

  rv = connect(*sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

  if (rv <0)
  {
    lewgLogError("Remote Socket not reachable\n");
    lewgSocketDestroy(sock);

    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSocketDestroy(int* socket)
{
  if (socket == 0)
    return LEWG_SUCCESS;

  if (*socket == 0)
    return LEWG_SUCCESS;

  lewgDestroyTunnel();
  close(*socket);

  *socket = 0;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgSocketSend(int* sock, const char* bytes, int length)
{

  if ((sock == 0) || (*sock == 0))
  {
    lewgLogError("Socket not initialized");
    lewgSocketDestroy(sock);

    return LEWG_ERROR;
  }
 
  int rv = 0;

  rv = send(*sock,bytes,length,0);

  if (rv != length)
  {
    // a transmission error is no need to destroy the socket.
    lewgLogError("Sending telegram failed");
    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}
*/
