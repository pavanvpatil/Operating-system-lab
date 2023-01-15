#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Error: Pass proper arguments\n");
        return 1;
    }

    argv = &argv[1];
    execvp(argv[0], argv);
    return 0;
}