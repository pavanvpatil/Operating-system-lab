#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int number = atoi(argv[argc - 1]);
    number = number / 2;
    printf("Half: Current process id: %d, Current result: %d\n", getpid(), number);

    if (argc <= 2)
        return 0;

    char num[11];
    sprintf(num, "%d", number);
    strcpy(argv[argc - 1], num);

    argv = &argv[1];
    execvp(argv[0], argv);
    return 0;
}