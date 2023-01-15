#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  char s[] = "Hello World";
  int pid, i;
  for (i = 0; s[i]!='\0'; i++)
  {
    pid = fork();
    if (pid == 0)
    {
      int process_id = getpid();
      printf("Character: %c Processid: %d\n", s[i], process_id);
    }
    else
    {
      sleep(rand() % 4 + 1);
      break;
    }
  }
  return 0;
}
