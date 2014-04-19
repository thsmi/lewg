#ifndef LEWG_IO_FILE_H_
  #define LEWG_IO_FILE_H_

  #include <errno.h>

  #include <stdio.h>
  #include <unistd.h>

  #include "./../utils/lewg.core.types.h"
  #include "./../utils/lewg.util.logger.h"

  typedef struct {
    FILE* handle;
  } lewgFileHandle_t;


  lewgReturn_t lewgDeleteFile(const char* file);
  lewgReturn_t lewgChangeDirectory(const char* directory);

  lewgReturn_t lewgOpenFile(lewgFileHandle_t* fh, const char* file, const char* mode);
  lewgReturn_t lewgCloseFile(lewgFileHandle_t* fh);

  lewgReturn_t lewgFileReadln(lewgFileHandle_t* fh, char* buffer, const int len);
  
  lewgReturn_t lewgFilePrint(lewgFileHandle_t* fh, const char* format, ... );
  lewgReturn_t lewgFileWrite(lewgFileHandle_t* fh, char* buffer, const int len);
#endif