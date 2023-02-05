rm -f /usr/src/minix/kernel/system.c
rm -f /usr/src/minix/servers/sched/schedule.c
mv schedule.c /usr/src/minix/servers/sched
mv system.c /usr/src/minix/kernel
make build MKUPDATE=yes -C /usr/src
