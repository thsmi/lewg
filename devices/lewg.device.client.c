#include "./../devices/lewg.device.client.h"

// TODO this should be stored in a config file...
//#ifndef LEWG_TUNNEL_CMD
//  #define LEWG_TUNNEL_CMD "dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost"
//#endif

//#ifndef LEWG_TUNNEL_USR
//  #define LEWG_TUNNEL_USR 0
  //#define LEWG_TUNNEL_USR "DROPBEAR_PASSWORD=root"
//#endif


// we need this only temporary until the ip is specified in the config file
//#ifndef LEWG_TUNNEL_IP
//  #define LEWG_TUNNEL_IP "127.0.0.1"
//#endif

//#ifndef LEWG_TUNNEL_PORT
//  #define LEWG_TUNNEL_PORT 3333
//#endif

lewgReturn_t lewgClientSend(const char* ip, int port, char* tunnel,  const char* bytes, int length)
{
  lewgReturn_t rv = LEWG_ERROR;
  lewgSocket_t sock;

  //char* const argv[] = {"/usr/bin/sh", "-c", LEWG_TUNNEL_CMD, NULL};
  //char* const envp[] = {LEWG_TUNNEL_USR, NULL};


  if (tunnel == NULL)
  {
    lewgLogInfo("Starting direct-tcp connection to: %s:%d \n",ip,port);
    rv = lewgSocketInit(&sock, ip, port);
  }
  else
  {
    lewgLogInfo("Connecting to  tunnel at %s:%d \n",ip,port);
    rv = lewgSocketInitEx(&sock, ip, port, tunnel, 0);
  }
  
  LEWG_ENSURE_SUCCESS(rv);
  //if (rv != LEWG_SUCCESS)
  //  return rv;

  rv = lewgSocketSend(&sock, "CODE\n", 5);
  rv = lewgSocketSend(&sock, bytes, length);

  lewgSocketDestroy(&sock);

  return rv;
}