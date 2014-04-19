
#include "lewg.io.serial.h"
#include <errno.h>

lewgReturn_t lewgInitPort(int* fd, const char* device)
{
  struct termios options;

  *fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

  if (*fd == -1)
  {
    lewgLogError("Unable to open %s - %s\n", device, strerror(errno));
    return LEWG_ERROR;
  }

  // Disable read delay, go asynchronous
  fcntl(*fd, F_SETFL, FNDELAY);

  tcgetattr(*fd, &options);

  // Set speed.
  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);

  // Enable Read
  options.c_cflag |= (CLOCAL | CREAD);

  // No Parity bit
  options.c_cflag &= ~PARENB;

  // 1 Stop bit
  options.c_cflag &= ~CSTOPB;

  // 8 Data bits
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  // Disable flow control
  options.c_cflag &= ~CRTSCTS;

  // Switch to raw mode
  options.c_lflag &= ~(ICANON | ECHO | ISIG);

  // Raw output
  options.c_oflag &= ~ OPOST;

  tcsetattr(*fd, TCSANOW, &options);

  return LEWG_SUCCESS;
}

lewgReturn_t lewgClosePort(int* fd)
{
  if (*fd == 0)
    return LEWG_SUCCESS;

  close(*fd);
  *fd = 0;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgReadChar(int fd, char* ch)
{
  ch[0] = 0;
  int rv = 0;

  rv = read(fd,ch,1);

  // Check if read would block, if so we can skip right here...
  if (rv <0 && (errno == EAGAIN || errno == EWOULDBLOCK))
    return LEWG_CONTINUE;

  // a 0 is an  EOF, which means the device is gone...
  // ... and it won't come back.
  if (rv == 0)
  {
    lewgLogError("EOF read on tty\n");
    return LEWG_ERROR;
  }


  // everything elese is a fatal error..
  if (rv < 0)
  {
    lewgLogError("Reading tty failed %d :  %s \n", errno, strerror(errno));
    return LEWG_ERROR;
  }

  if (ch[0] == 0)
    return LEWG_CONTINUE;

//  printf("Got %c.\n", ch[0]);

  return LEWG_SUCCESS;
}

lewgReturn_t lewgWriteChar(int fd, char* ch)
{
  int rv = 0;

  rv = write(fd,ch,1);

  if (rv < 0)
  {
    lewgLogError("Error writing %c to serial device", &ch[0]);
    return LEWG_ERROR;
  }

  return LEWG_SUCCESS;
}

