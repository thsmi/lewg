gcc -Wall \
  \
  -DLEWG_LOGFILE=\"/var/log/lewg.server.log\" \
  \
  ./utils/lewg.core.string.c\
  ./utils/lewg.core.main.deamon.c\
  ./utils/lewg.core.singleton.c\
  ./utils/lewg.core.daemon.c\
  ./utils/lewg.core.args.c\
  ./apps/server/lewg.server.c \
  ./io/lewg.io.ethernet.server.c \
  ./io/lewg.io.file.c\
  ./utils/lewg.util.logger.c \
  ./code/lewg.code.system.c\
  ./code/lewg.code.localstore.c\
  ./utils/lewg.util.system.c\
  ./utils/lewg.util.script.c\
  ./io/lewg.io.directory.c\
  \
  -lbsd\
  \
  -o lewg.server
