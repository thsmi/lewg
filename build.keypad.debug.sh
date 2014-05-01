gcc  \
 -Wall \
 -DLEWG_IP=\"127.0.0.1\" \
 -DLEWG_EMULATE_KEYPAD\
 -DLEWG_EMULATE_SYSTEM_CALL\
 -DLEWG_LOGFILE=\"/var/log/lewg.client.log\" \
 -DLEWG_NO_DAEMON\
 \
 ./utils/lewg.core.main.deamon.c\
 ./utils/lewg.core.singleton.c\
 ./utils/lewg.core.daemon.c\
 ./utils/lewg.core.args.c\
 ./client/lewg.client.c \
 ./io/lewg.io.tunnel.c \
 ./io/lewg.io.ethernet.client.c \
 ./io/lewg.io.serial.c \
 ./utils/lewg.util.logger.c \
 ./utils/lewg.util.system.c \
 ./code/lewg.code.system.c\
 ./devices/lewg.device.keypad.c\
 ./devices/lewg.device.client.c\
 \
 -lbsd\
 \
 -o lewg.client\

