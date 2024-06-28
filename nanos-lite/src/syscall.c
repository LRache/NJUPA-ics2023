#include <common.h>
#include "syscall.h"
#include <inttypes.h>

int sys_write(int fd, const void *buf, size_t count);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  uintptr_t a0 = 0;
  switch (a[0]) {
    case SYS_exit:  halt(a[1]);       break;
    case SYS_yield: yield();          break;
    case SYS_write: 
      a0 = sys_write(a[0], (void*)a[1], a[2]); 
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  c->GPRx = a0;
}

int sys_write(int fd, const void *buf, size_t count) {
  const char *buffer = (const char *)buf;
  printf("%s\n", buffer);
  if (fd == 1 || fd == 2) {
    for (int i = 0; i < 13; i++) {
      putch(buffer[i]);
    }
    return count;
  }
  return -1;
}
