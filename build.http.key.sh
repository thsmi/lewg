gcc \
  -Wall\
  -DLEWG_LOGFILE=\"/var/log/lewg.http.key.log\" \
  \
  ./core/lewg.core.main.webapp.c\
  ./utils/lewg.util.logger.c \
  ./io/lewg.io.stdio.c\
  ./io/lewg.io.directory.c\
  ./io/lewg.io.file.c\
  ./utils/lewg.core.string.c\
  ./code/lewg.code.localstore.c\
  ./apps/http/lewg.http.key.c \
  \
  -o ./release/lewg.http.key

#  -DLEWG_LOGFILE=\"/var/log/lewg.client.simulator.log\" \
#  -DLEWG_TUNNEL_CMD=\"dbclient -T -N -L 3333:127.0.0.1:8081 -l root localhost \" \

cp ./release/lewg.http.key /srv/http/cgi-bin