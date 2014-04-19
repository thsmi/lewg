#ifndef LEWG_IO_DIRECTORY_H_
  #define LEWG_IO_DIRECTORY_H_

  #include <errno.h>
  #include "./../utils/lewg.core.types.h"

  #include <dirent.h>

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "./../utils/lewg.util.logger.h"

  #include <stdint.h>
  #include <unistd.h>
  #include <sys/stat.h>
  
  #include "./lewg.io.file.h"

  typedef struct {
    DIR* handle;
  } lewgDirectoryHandle_t;

  typedef struct {
    char* name;
    uint8_t type;
  } lewgDirectoryItem_t;


  lewgReturn_t lewgChangeDirectory(const char* directory);
  
  lewgReturn_t lewgCreateDirectory(const char* directory);
  lewgReturn_t lewgDeleteDirectory(const char* directory);
  lewgReturn_t lewgRenameDirectory(const char* old, const char* new);
  
  lewgReturn_t lewgOpenDirectory(lewgDirectoryHandle_t* dh, const char* path);
  lewgReturn_t lewgNextDirectory(lewgDirectoryHandle_t* dh, lewgDirectoryItem_t* item);
  lewgReturn_t lewgCloseDirectory(lewgDirectoryHandle_t* dh);
  
  lewgReturn_t lewgIsDirectory(lewgDirectoryItem_t* item);
  lewgReturn_t lewgIsFile(lewgDirectoryItem_t* item);
  lewgReturn_t lewgIsSymlink(lewgDirectoryItem_t* item);

#endif