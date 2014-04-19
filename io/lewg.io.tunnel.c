#include "./../io/lewg.io.tunnel.h"

extern char **environ;

lewgReturn_t lewgInitTunnel(lewgTunnel_t* tunnel, char* cmd, char* env)
{
  char* const argv[] = {"/usr/bin/sh", "-c", cmd, NULL};
  char* const envp[] = {env, NULL};
  //char* const envp[] = {NULL};

  tunnel->pid = 0;
   
  // No Args, no tunnel...
  if (cmd == 0)
    return LEWG_SUCCESS;
  
  tunnel->pid = fork();

  if (tunnel->pid == 0)
  {
    execve(argv[0], argv, (env != NULL)? envp: environ);
    
    lewgLogError("Opening tunnel failed:  %s \n", strerror(errno));
    lewgDestroyLog();
    // exec only retuns on error...
    _exit(EXIT_FAILURE);
  }

  lewgLogInfo("Client Pid  %jd for %s .\n",((intmax_t) tunnel->pid),cmd);
  
  // TODO: The tunnel needs some time to be established. Instead of sleeping...
  // ... it would be better to implement into socket connect an autmatic...
  // ... retry with a timelimit. The two secons are just a rough guess to be...
  // ... on the safe side.
  
  sleep(2);

  int status;
  pid_t rv = 0;

  rv = waitpid(tunnel->pid, &status, WNOHANG);

  // Is process terminated for some reason?
  if ((rv > 0) && (WIFEXITED(status) || WIFSIGNALED(status)))
  {
    lewgLogError("Tunnel died during initialisation\n");
    return LEWG_ERROR;
  }
  return LEWG_SUCCESS;
}

lewgReturn_t lewgDestroyTunnel(lewgTunnel_t* tunnel)
{
  int status;
  pid_t rv = 0;

  if ((tunnel->pid == -1) || (tunnel->pid == 0))
    return LEWG_SUCCESS;

  // TODO: The tunnel might delay delivery. After sending a message we need to give...
  // ... him some time or implement a serverside feedback. Which indicates when it's... 
  // ... safe to close the tunnel.
  sleep(1);

  rv = waitpid(tunnel->pid, &status, WNOHANG);

  // Is process terminated for some reason?
  if ((rv > 0) && (WIFEXITED(status) || WIFSIGNALED(status)))
    return LEWG_SUCCESS;

  lewgLogInfo("Killing process pid %jd %d.\n",((intmax_t) tunnel->pid), rv);

  if (kill(tunnel->pid, SIGINT) != 0)
  {
    lewgLogError("Failed to kill process : %s \n", strerror(errno));
    return LEWG_ERROR;
  }

  // Kill is triggered we need to wait until process is gone...
  rv = waitpid(tunnel->pid,0,0);

  if (rv < 0)
  {
    lewgLogError("Failed to kill process giving up\n");
    return LEWG_ERROR;
  }

  lewgLogInfo("process terminated\n");
  
  return LEWG_SUCCESS;
}