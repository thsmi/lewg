#include "lewg.code.localstore.h"

static lewgReturn_t lewgIsActive(const char* buffer)
{
  lewgReturn_t rv = LEWG_ERROR;
  time_t begin;
  time_t end;

  rv = lewgParseInterval(&buffer[0], &begin, &end);
  LEWG_ENSURE_SUCCESS(rv);

  // Check if it's a fast forward...
  if (difftime(begin, time(0)) > 0)
    return LEWG_ERROR;

  // ... the check if it expired.
  if (difftime(end, time(0)) < 0)
    return LEWG_ERROR;

  return LEWG_SUCCESS;


  // Valid enties are:
  // yyyy-mm-dd hh:mm UNTIL yyyy-mm-dd hh:mm (GMT)
  // yyyy-mm-dd hh:mm UNTIL yyyy-mm-dd hh:mm (LOCAL)
  // yyyy-mm-dd hh:mm UNTIL yyyy-mm-dd hh:mm
  // 012345678901234567890123456789012345678901234567890
  
//  if (strlen(&buffer[0]) < 39)
//    return LEWG_ERROR;
//
//  struct tm begin;
//  rv = lewgParseTime(&buffer[0], &begin);
//
//  LEWG_ENSURE_SUCCESS(rv);
//
//  if (memcmp(" UNTIL ",&buffer[16],7))
//    return LEWG_ERROR;

//  struct tm end;
//  rv = lewgParseTime(&buffer[23], &end);

//  LEWG_ENSURE_SUCCESS(rv);

//  if ((buffer[39] == '\n') || (memcmp(" (GMT)\n",&buffer[39],7) == 0))
//  {
//    // Check if it's a fast forward...
//    if (difftime(timegm(&begin), time(0)) > 0)
//      return LEWG_ERROR;

//    // ... the check if it expired.
//    if (difftime(timegm(&end),time(0)) < 0 )
//      return LEWG_ERROR;

//    return LEWG_SUCCESS;
//  }

//  if (memcmp(" (LOCAL)\n", &buffer[39], 9) == 0)
//  {
//    if (difftime(mktime(&begin),time(0)) > 0)
//      return LEWG_ERROR;

//    if (difftime(mktime(&end), time(0)) < 0)
//      return LEWG_ERROR;

//    return LEWG_SUCCESS;
//  }
  
  return LEWG_ERROR;
}

//    # SOMETHING...
//    ACTIVE: 2013-01-21 15:00 UNTIL 2013-01-22 18:00
//    ACTIVE: 2013-01-22 16:30 UNTIL 2013-01-22 17:00 (GMT)
//    ACTIVE: 2013-01-23 17:30 UNTIL 2013-01-23 18:00 (LOCAL)
//
//    # Execute a command...
//    EXECUTE: reboot
//    EXECUTE: shutdown -h now

// *12345678#  
// *12345678# 2013-01-22 16:30 03600\n
// tm_min(0-59) tm_hour(0-23) tm_mday(1-31) tm_mon(0-11) tm_year
// gmtime
// year month day hour min  min

// Checks if the code contains "safe characters"
lewgReturn_t lewgSanatizeCode(const char* buffer, lewgLocalCode_t* code)
{
  int i=0;

  // ... the first character has to be a star 
  if (buffer[0] != '*')
  {
    lewgLogInfo("* expected but found %.*s \n", LEWG_MAX_CODE_LENGTH, &buffer[0]);
    return LEWG_ERROR;
  }

  for (i=1; i<LEWG_MAX_CODE_LENGTH; i++)
  {
    switch(buffer[i])
    {
      case '0': case '1':
      case '2': case '3':
      case '4': case '5':
      case '6': case '7':
      case '8': case '9':
      case 'A': case 'B':
      case 'C':
        continue;
    }

    break;
  }

  if (i < 3)
  {
    lewgLogInfo("Code is empty %.*s \n", LEWG_MAX_CODE_LENGTH, &buffer[0]);
    return LEWG_ERROR;
  }

  if (i == LEWG_MAX_CODE_LENGTH)
  {
    lewgLogInfo("Code too long %.*s \n", LEWG_MAX_CODE_LENGTH, &buffer[0]);
    return LEWG_ERROR;
  }

  if ((buffer[i] != '#') && (buffer[i] != '.'))
  {
    lewgLogInfo("Terminating # or . expected but found %.*s \n", LEWG_MAX_CODE_LENGTH, &buffer[0]);
    return LEWG_ERROR;
  }

  // copy the code, we need to drop the *
  memcpy(&code->code[0],&buffer[1],i-1);
  // ... as well as the # or .
  code->code[i-1] = '\0';

  // extract the type...
  code->type = buffer[i];

  // drop the * character...
  lewgLogInfo("Clean key %s length %d\n",code->code,strlen(code->code));
  return LEWG_SUCCESS;
}

lewgReturn_t lewgValidateLifeCycle()
{
  const int LENGTH = 1024;
  char buffer[LENGTH];

  lewgReturn_t rv = LEWG_SUCCESS;
  lewgFileHandle_t fh;

  rv = lewgOpenFile(&fh, "lifecycle","r");
  LEWG_ENSURE_SUCCESS(rv);

  while (lewgFileReadln(&fh, &buffer[0], LENGTH) == LEWG_SUCCESS)
  {
    lewgLogInfo("Parsing Line: %s\n",&buffer[0]);

    rv = lewgIsComment(&buffer[0]);
    if (rv == LEWG_SUCCESS)
      continue;

    rv = lewgIsWhiteSpace(&buffer[0]);
    if (rv == LEWG_SUCCESS)
       continue;

    lewgLogInfo("Evaluating Timestamp %s",&buffer[0]);

    rv = lewgIsActive(&buffer[0]);

    if (rv != LEWG_SUCCESS)
      continue;

    lewgCloseFile(&fh);
    return LEWG_SUCCESS;
  }

  lewgCloseFile(&fh);
  return LEWG_ERROR;
}


//  LEWG_ENSURE_SUCCESS_INFO(rv,"No active code found");
//  lewgLogInfo("Key in file  %.*s matches code %.*s\n",length,&buffer[0], length, &code[0]);
//  lewgLogError("Key file not found, path %s invalid\n", keyfile);

