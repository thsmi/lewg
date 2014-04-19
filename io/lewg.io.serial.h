/*
 * lewg.serial.h
 *
 *  Created on: 05.09.2013
 *      Author: admin
 */

#ifndef LEWG_IO_SERIAL_H_
  #define LEWG_IO_SERIAL_H_

  #include "stdio.h"
  #include "string.h"
  #include "unistd.h"
  #include "fcntl.h"
  #include "errno.h"
  #include "termios.h"

  #include "../utils/lewg.util.logger.h"
  #include "../utils/lewg.core.types.h"

  lewgReturn_t lewgInitPort(int* fd, const char* device);
  lewgReturn_t lewgClosePort(int* fd);
  lewgReturn_t lewgReadChar(int fd, char* ch);
  lewgReturn_t lewgWriteChar(int fd, char* ch);

#endif /* LEWG_SERIAL_H_ */
