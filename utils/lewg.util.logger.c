
#include "lewg.util.logger.h"

#if defined(LEWG_SYSLOG)

  #include <syslog.h>
  
  //setlogmask(LOG_UPTO(LOG_NOTICE));

  lewgReturn_t  lewgInitLog()
  {
    openlog("lewg", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    atexit(lewgDestroyLog);

    return LEWG_SUCCESS;
  }

  void lewgDestroyLog()
  {
    closelog();
  }

  lewgReturn_t lewgLog(lewgLogType_t type, const char* format, ...)
  {
    va_list args;
    va_start(args, format);

    if (type == LEWG_LOG_INFO)
      vsyslog(LOG_INFO, format, args);

    if (type == LEWG_LOG_ERROR)
      vsyslog(LOG_ERR, format, args);

    va_end(args);

    return LEWG_SUCCESS;
  }

#elif defined(LEWG_LOGFILE)

  #include <stdio.h>
  #include <unistd.h>
  #include <stdint.h>

  static FILE* lewgLogFile = 0;

  lewgReturn_t lewgInitLog()
  {
    if (lewgLogFile != 0)
      return LEWG_ERROR;
    
    lewgLogFile = fopen(LEWG_LOGFILE,"a");
    
    atexit(lewgDestroyLog);
    return LEWG_SUCCESS;
  }

  void lewgDestroyLog()
  {
    if (lewgLogFile == 0)
      return;
      
    fclose(lewgLogFile);
    return;
  }


  lewgReturn_t lewgLog(lewgLogType_t type, const char* format, ...)
  {
    time_t now;
    struct tm* timeptr;

    if (lewgLogFile == 0)
      return LEWG_ERROR;

    fprintf(lewgLogFile, "[%jd ", (intmax_t) getpid());

    if (type == LEWG_LOG_INFO)
      fprintf(lewgLogFile, "INFO  ");

    if (type == LEWG_LOG_ERROR)
      fprintf(lewgLogFile, "ERROR ");


    time(&now);
    timeptr = gmtime(&now);

    fprintf(lewgLogFile, "%d-%.2d-%.2d %.2d:%.2d:%.2d] ",
      1900 + timeptr->tm_year,  timeptr->tm_mon + 1, timeptr->tm_mday, 
      timeptr->tm_hour,timeptr->tm_min, timeptr->tm_sec );

    va_list args;
    va_start(args,format);

    vfprintf(lewgLogFile,format,args);

    va_end(args);

    fflush(lewgLogFile);

    return LEWG_SUCCESS;
  }

#else

  #include <stdio.h>

  lewgReturn_t lewgInitLog()
  {
    return LEWG_SUCCESS;
  }

  void lewgDestroyLog()
  {
    return;
  }

  lewgReturn_t lewgLog(lewgLogType_t type, const char* format, ...)
  {
    time_t now;
    struct tm* timeptr;


    if (type == LEWG_LOG_INFO)
      printf("[INFO  ");
      
    if (type == LEWG_LOG_ERROR)
      printf("[ERROR ");
      

    time(&now);
    timeptr = gmtime(&now);

    printf("%d-%.2d-%.2d %.2d:%.2d:%.2d] ",
      1900 + timeptr->tm_year,  timeptr->tm_mon + 1, timeptr->tm_mday, 
      timeptr->tm_hour,timeptr->tm_min, timeptr->tm_sec );

    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);

    return LEWG_SUCCESS;
  }

#endif
