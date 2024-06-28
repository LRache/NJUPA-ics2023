#include <common.h>
#include "syscall.h"
#include <inttypes.h>

int sys_write(int fd, const void *buf, size_t count);
int sys_brk();

void do_syscall(Context *c) {
  uintptr_t s = c->GPR1;
  uintptr_t arg[3];
  arg[0] = c->GPR2;
  arg[1] = c->GPR3;
  arg[2] = c->GPR4;

  uintptr_t r = 0;
  switch (s) {
    case SYS_exit:  halt(arg[0]); break;
    case SYS_yield: yield();      break;
    case SYS_write: 
      r = sys_write(arg[0], (void*)arg[1], arg[2]); break;
    case SYS_brk:
      r = sys_brk();
    default: panic("Unhandled syscall ID = %d", s);
  }
  c->GPRx = r;
}

int sys_write(int fd, const void *buf, size_t count) {
  const char *buffer = (const char *)buf;
  if (fd == 1 || fd == 2) {
    for (int i = 0; i < count; i++) {
      putch(buffer[i]);
    }
    Log("%u", count);
    return count;
  }
  return -1;
}

int sys_brk() {
  return 0;
}
