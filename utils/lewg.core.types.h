#ifndef LEWG_CORE_TYPES_H_
  #define LEWG_CORE_TYPES_H_
  
  typedef enum
  {
     LEWG_SUCCESS = 1, 
     LEWG_CONTINUE = 2,
     LEWG_ERROR = -1, 
  } lewgReturn_t;

  #define LEWG_ENSURE_SUCCESS(X) \
    if ((X) < 0) return (X)

  #define LEWG_ENSURE_SUCCESS_INFO(X, ... ) \
    if ((X) < 0) { lewgLogInfo(__VA_ARGS__); return (X); }

  #define LEWG_ENSURE_SUCCESS_ERROR(X, ... )  \
    if ((X) < 0) { lewgLogError(__VA_ARGS__); return (X); }

#endif