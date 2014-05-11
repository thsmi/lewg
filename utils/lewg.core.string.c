#include "lewg.core.string.h"

lewgReturn_t lewgIsWhiteSpace(const char* buffer)
{
  int len = strlen(&buffer[0]);

  while (len--)
    if (!isspace(buffer[len]))
      return LEWG_ERROR;

  return LEWG_SUCCESS;
}

lewgReturn_t lewgIsComment(const char* buffer)
{
  if (buffer[0] == '#')
    return LEWG_SUCCESS;

  return LEWG_ERROR;
}


lewgReturn_t lewgParseInterval(const char* buffer, time_t* begin, time_t* end)
{
  lewgReturn_t rv = LEWG_ERROR;

  struct tm timeBegin;
  struct tm timeEnd;

  if (strlen(&buffer[0]) < 39)
    return LEWG_ERROR;

  rv = lewgParseTime(&buffer[0], &timeBegin);
  LEWG_ENSURE_SUCCESS(rv);

  if (memcmp(" UNTIL ",&buffer[16],7))
    return LEWG_ERROR;

  rv = lewgParseTime(&buffer[23], &timeEnd);
  LEWG_ENSURE_SUCCESS(rv);

  if ((buffer[39] == '\n') || (memcmp(" (GMT)\n",&buffer[39],7) == 0))
  {
    *begin = timegm(&timeBegin);
    *end = timegm(&timeEnd);

    // ensure the dates could be converted...
    if ( (*begin == -1) || (*end == -1))
      return LEWG_ERROR;

    return LEWG_SUCCESS;
  }


  if (memcmp(" (LOCAL)\n",&buffer[39],9) == 0)
  {
    *begin = mktime(&timeBegin);
    *end = mktime(&timeEnd);

    // ensure the dates could be converted...
    if ( (*begin == -1) || (*end == -1))
      return LEWG_ERROR;

    return LEWG_SUCCESS;
  }

  return LEWG_ERROR;
}


// # SOMETHING...
// 2013-01-21 15:00 UNTIL 2013-01-22 18:00
// 2013-01-22 16:30 UNTIL 2013-01-22 17:00 (GMT)
// 2013-01-23 17:30 UNTIL 2013-01-23 18:00 (LOCAL)
lewgReturn_t lewgParseTime(const char* buffer, struct tm* timeinfo)
{
  // the dateformat is yyyy-mm-dd hh:mm
  // Extract the year...
  if (!isdigit(buffer[0]))
    return LEWG_ERROR;

  timeinfo->tm_year = atoi(&buffer[0])-1900;

  if (buffer[4] != '-')
    return LEWG_ERROR;

  // ... then extact the month ...
  if (!isdigit(buffer[5]))
    return LEWG_ERROR;

  timeinfo->tm_mon = atoi(&buffer[5])-1;

  if (buffer[7] != '-')
    return LEWG_ERROR;

  // ... and the day ...
  if (!isdigit(buffer[8]))
    return LEWG_ERROR;

  timeinfo->tm_mday = atoi(&buffer[8]);

  if (buffer[10] != ' ')
    return LEWG_ERROR;

  // ... finally the hours ...
  if (!isdigit(buffer[11]))
    return LEWG_ERROR;

  timeinfo->tm_hour = atoi(&buffer[11]);

  if (buffer[13] != ':')
    return LEWG_ERROR;

  // ... and seconds.
  if (!isdigit(buffer[14]))
    return LEWG_ERROR;

  timeinfo->tm_min = atoi(&buffer[14]);
  timeinfo->tm_sec = 0;

  timeinfo->tm_isdst = -1;

  return LEWG_SUCCESS;
}
