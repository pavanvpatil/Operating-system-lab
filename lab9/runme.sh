cp open.c /usr/src/minix/servers/vfs/open.c
cp link.c /usr/src/minix/servers/vfs/link.c
cp read.c /usr/src/minix/servers/vfs/read.c
make build MKUPDATE=yes -C /usr/src
