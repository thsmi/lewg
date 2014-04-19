#include "lewg.io.file.h"

lewgReturn_t lewgDeleteFile(const char* file)
{
  int rv;

  rv = remove(&file[0]);

  if (rv == 0)
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}

lewgReturn_t lewgOpenFile(lewgFileHandle_t* fh, const char* file, const char* mode)
{
  fh->handle = fopen(file,mode);

  if (fh->handle > 0)
    return LEWG_SUCCESS;

  if (errno != ENOENT)
  {
    lewgLogInfo("Failed to open file %s \n",&file[0]);
    return LEWG_ERROR;
  }

  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
    lewgLogInfo("Current working dir: %s\n", cwd);

  lewgLogInfo("File no found >>%s<< \n",&file[0]);

  return LEWG_ERROR;
}

lewgReturn_t lewgFileReadln(lewgFileHandle_t* fh, char* buffer, const int len)
{
  char* rv = 0;
  rv = fgets(&buffer[0], len, fh->handle);

  // on success fgets return a pointer to our buffer
  // in case of an error or the eof was reached, null is returned
  if (rv == 0)
    return LEWG_ERROR;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgFileWrite(lewgFileHandle_t* fh, char* buffer, const int len)
{
  size_t rv = 0;
  
  rv = fwrite(&buffer[0], 1, len, fh->handle);
  
  if (rv == len)
    return LEWG_SUCCESS;
  
  return LEWG_ERROR;
}

lewgReturn_t lewgFilePrint(lewgFileHandle_t* fh, const char* format, ... )
{
  int rv = 0;
  va_list args;
  
  va_start(args, format);

  rv = vfprintf(fh->handle, format, args);

  va_end(args);

  if (rv > 0)
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}

lewgReturn_t lewgFlushFile(lewgFileHandle_t* fh)
{
  fflush(fh->handle);
  return LEWG_SUCCESS;
}

lewgReturn_t lewgCloseFile(lewgFileHandle_t* fh)
{
  fclose(fh->handle);
  fh->handle = 0;

  return LEWG_SUCCESS;
}
