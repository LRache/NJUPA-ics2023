#include <common.h>
#include "syscall.h"
#include "fs.h"

int sys_open(char *pathname, int flags, int mode);
size_t sys_read(int fd, void *buf, size_t len);
int sys_write(int fd, const void *buf, size_t count);
int sys_close(int fd);
size_t sys_lseek(int fd, off_t offset, int whence);
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
    case SYS_open:
      r = sys_open((char *)arg[0], arg[1], arg[2]); break;
    case SYS_read:
      r = sys_read(arg[0], (void *)arg[1], arg[2]); break;
    case SYS_write: 
      r = sys_write(arg[0], (void *)arg[1], arg[2]); break;
    case SYS_close:
      r = sys_close(arg[0]); break;
    case SYS_lseek:
      r = sys_lseek(arg[0], arg[1], arg[2]); break;
    case SYS_brk:
      r = sys_brk(); break;
    default: panic("Unhandled syscall ID = %d", s);
  }
  c->GPRx = r;
}

int sys_open(char *pathname, int flags, int mode) {
  return fs_open(pathname, flags, mode);
}

size_t sys_read(int fd, void *buf, size_t len) {
  return fs_read(fd, buf, len);
}

int sys_write(int fd, const void *buf, size_t count) {
    return fs_write(fd, buf, count);
}

int sys_close(int fd) {
  return fs_close(fd);
}

size_t sys_lseek(int fd, off_t offset, int whence) {
  return fs_lseek(fd, offset, whence);
}

int sys_brk() {
  return 0;
}
