#include <am.h>
#include <syscall.h>

bool ioe_init() {
  return true;
}

int _ioe_read(int reg, void *buf);
int _ioe_write(int reg, void *buf);

void ioe_read (int reg, void *buf) {
  _ioe_read(reg, buf);
}
void ioe_write(int reg, void *buf) {
  _ioe_write(reg, buf);
}
