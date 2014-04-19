#ifndef LEWG_DEVICE_CLIENT_H_
  #define LEWG_DEVICE_CLIENT_H_
  
  #include "./../io/lewg.io.ethernet.client.h"
  #include "./../io/lewg.io.tunnel.h"
  #include "./../utils/lewg.core.args.h"
  
  lewgReturn_t lewgClientSend(const char* ip, int port, char* tunnel, const char* bytes, const int length);
  
#endif
