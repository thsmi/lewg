#ifndef LEWG_DEVICE_TUNNEL_H_
  #define LEWG_DEVICE_TUNNEL_H_
  
  #include <unistd.h>
  #include <stdlib.h> 
  #include <stdint.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <string.h>
  #include <errno.h>
  
  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"
  
  typedef struct {
    pid_t pid;
  } lewgTunnel_t;

  //    char* argv[] = {"/bin/echo", "Test",NULL};
  //    char* envp[] = {NULL};

  // 1. Open tunnel
  // 2. Connect to port
  // 3. timeout or finished transmission?
  // 4. destroy

  lewgReturn_t lewgInitTunnel(lewgTunnel_t* tunnel, char* arg, char* env);

  lewgReturn_t lewgDestroyTunnel(lewgTunnel_t* tunnel);

#endif
