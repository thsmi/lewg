#include "lewg.io.directory.h"

lewgReturn_t lewgChangeDirectory(const char* directory)
{
  int rv = 0;
  
  rv = chdir(directory);
  if ( rv == 0)
    return LEWG_SUCCESS;

  lewgLogError("Could not change directory %s \n", &directory[0]);
  return LEWG_ERROR;
}

lewgReturn_t lewgCreateDirectory(const char* directory)
{
  int rv = 0;

  rv = mkdir(&directory[0],0777);

  if (rv == 0)
    return LEWG_SUCCESS;

  lewgLogError("Could not create directory %s, %s\n", &directory[0],strerror(errno));
  return LEWG_ERROR;
}

lewgReturn_t lewgRenameDirectory(const char* old, const char* new)
{
  int rv = 0;

  rv = rename(old, new);

  if (rv == 0)
    return LEWG_SUCCESS;

  lewgLogError("Could not rename %s to %s \n", &old[0], &new[0]);
  return LEWG_ERROR;
}


lewgReturn_t lewgDeleteDirectory(const char* directory)
{
  lewgReturn_t rv;

  lewgDirectoryHandle_t dh;
  lewgDirectoryItem_t item;

  // Step 1: delete all files and symlinks...
  rv = lewgOpenDirectory(&dh, &directory[0]);
  LEWG_ENSURE_SUCCESS(rv);

  lewgLogInfo("Opening directory %s\n",&directory[0]);
  lewgChangeDirectory(&directory[0]);


  while (lewgNextDirectory(&dh, &item) == LEWG_SUCCESS)
  {
    lewgLogInfo("Opening directory %s\n",&item.name[0]);
    // Skip the system dirs...
    if (memcmp(".\0", &item.name[0],2) == 0)
      continue;

    if (memcmp("..\0", &item.name[0],3) == 0)
      continue;

    if (lewgIsDirectory(&item) == LEWG_SUCCESS)
    {
      lewgLogInfo("Delete directory %s \n", &item.name[0]);
      lewgDeleteDirectory(&item.name[0]);
      continue;
    }

    if (lewgIsFile(&item) == LEWG_SUCCESS)
    {
      lewgLogInfo("Delete file %s \n", &item.name[0]);
      lewgDeleteFile(&item.name[0]);

      continue;
    }

    if (lewgIsSymlink(&item) == LEWG_SUCCESS)
    {
      lewgLogInfo("Delete symlink %s \n", &item.name[0]);
      lewgDeleteFile(&item.name[0]);

      continue;
    }

    // It's an unknow file type...
    lewgLogError("Unknown file type %s \n", &item.name[0]);
 
    lewgChangeDirectory("..");
    lewgCloseDirectory(&dh);
    return LEWG_ERROR;
  }

  lewgChangeDirectory("..");
  lewgCloseDirectory(&dh);

  lewgLogInfo("Delete directory %s \n", &directory[0]);
  rmdir(&directory[0]);
  return LEWG_SUCCESS;
}

lewgReturn_t lewgOpenDirectory(lewgDirectoryHandle_t* dh, const char* path)
{
  if (dh == 0)
    return LEWG_ERROR;

  dh->handle = opendir(path);

  if (dh->handle != 0)
    return LEWG_SUCCESS;
  

//    if (errno != 0)
//      printf("HTTP/1.1 500 %s\r\n\r\n%s\r\n",strerror(errno),strerror(errno));
//    else
//      printf("HTTP/1.1 500\r\n\r\nUnknown error\r\n");

  return LEWG_ERROR;
}

lewgReturn_t lewgNextDirectory(lewgDirectoryHandle_t* dh, lewgDirectoryItem_t* item)
{
  struct dirent* next = 0;

  next = readdir(dh->handle);

  if (next == 0)
    return LEWG_ERROR;

  item->type = next->d_type;
  item->name = &(next->d_name[0]);

  return LEWG_SUCCESS;
}

lewgReturn_t lewgCloseDirectory(lewgDirectoryHandle_t* dh)
{
  closedir(dh->handle);
  dh->handle = 0;

  return LEWG_SUCCESS;
}


lewgReturn_t lewgIsDirectory(lewgDirectoryItem_t* item)
{
  if (item->type == DT_DIR)
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}

lewgReturn_t lewgIsFile(lewgDirectoryItem_t* item)
{
  if (item->type == DT_REG)
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}

lewgReturn_t lewgIsSymlink(lewgDirectoryItem_t* item)
{
  if (item->type == DT_LNK)
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}
