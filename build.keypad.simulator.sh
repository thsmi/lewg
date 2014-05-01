gcc \
  -Wall\
  \
  ./core/lewg.core.main.app.c\
  ./utils/lewg.util.logger.c \
  ./utils/lewg.core.args.c\
  ./apps/keypad/lewg.keypad.simulator.c \
  ./io/lewg.io.serial.c \
  \
  -lbsd\
  \
  -o lewg.simulator

#  -DLEWG_LOGFILE=\"/var/log/lewg.client.simulator.log\" \
#  -DLEWG_TUNNEL_CMD=\"dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost \" \
