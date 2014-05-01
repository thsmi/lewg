gcc -Wall \
  \
  -DLEWG_LOGFILE=\"/var/log/lewg.opener.log\" \
  -DLEWG_EMULATE_STRIKE\
  \
  ./core/lewg.core.main.app.c\
  ./utils/lewg.core.args.c\
  ./apps/opener/lewg.opener.c \
  ./io/lewg.io.serial.c\
  ./utils/lewg.util.logger.c \
  ./devices/lewg.device.strike.c\
  \
  -lbsd\
  \
  -o lewg.opener
