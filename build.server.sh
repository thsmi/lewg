gcc -Wall \
  \
  -DLEWG_LOGFILE=\"/var/log/lewg.server.log\" \
  \
  ./core/lewg.core.main.daemon.c\
  ./utils/lewg.core.args.c\
  ./utils/lewg.core.singleton.c\
  ./utils/lewg.core.daemon.c\
  ./utils/lewg.core.string.c\
  ./utils/lewg.util.logger.c \
  ./utils/lewg.util.script.c \
  ./io/lewg.io.file.c\
  ./io/lewg.io.directory.c\
  ./apps/server/lewg.server.c \
  ./io/lewg.io.serial.c \
  ./io/lewg.io.ethernet.server.c \
  ./devices/lewg.device.strike.c\
  ./code/lewg.code.system.c\
  ./code/lewg.code.localstore.c\
  ./utils/lewg.util.system.c\
  \
  -lbsd\
  \
  -o lewg.server
