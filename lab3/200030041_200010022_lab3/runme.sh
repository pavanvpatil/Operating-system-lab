rm -f /usr/src/minix/servers/sched/schedule.c
mv schedule.c /usr/src/minix/servers/sched/
make build MKUPDATE=yes -C /usr/src/
