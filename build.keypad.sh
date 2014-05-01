gcc \
  -Wall\
  -DLEWG_LOGFILE=\"/var/log/lewg.client.log\" \
  "-DLEWG_TUNNEL_CMD=\"dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost \""\
  "-DLEWG_TUNNEL_USR=\"DROPBEAR_PASSWORD=root\"" \
  \
  ./core/lewg.core.main.daemon.c\
  ./utils/lewg.util.logger.c \
  ./utils/lewg.core.args.c\
  ./utils/lewg.core.singleton.c\
  ./utils/lewg.core.daemon.c\
  ./apps/keypad/lewg.keypad.client.c \
  ./io/lewg.io.tunnel.c \
  ./io/lewg.io.ethernet.client.c \
  ./io/lewg.io.serial.c \
  ./utils/lewg.util.system.c \
  ./code/lewg.code.system.c\
  ./devices/lewg.device.keypad.c\
  ./devices/lewg.device.client.c\
  \
  -lbsd\
  \
  -o lewg.keypad.client

#  -DLEWG_IP=\"192.168.178.246\" \
#  -DLEWG_TUNNEL_CMD=\"dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost \" \
