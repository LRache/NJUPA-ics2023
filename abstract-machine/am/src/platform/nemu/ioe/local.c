#include <am.h>
#include <nemu.h>
#include <klib.h>

#define CTL_OP_ADDR   (LOCAL_CTL_ADDR +  0)
#define CTL_FD_ADDR   (LOCAL_CTL_ADDR +  4)
#define CTL_ARG_ADDR  (LOCAL_CTL_ADDR +  8)
#define CTL_EXE_ADDR  (LOCAL_CTL_ADDR + 12)

void __am_local_open(AM_LOCAL_OPEN_T *c) {
    *(uint32_t *)CTL_OP_ADDR = 0;
    *(uint32_t *)CTL_ARG_ADDR = c->mode;
    strcpy((char *)LOCAL_ARG_ADDR, c->pathname);
    *(uint32_t *)CTL_EXE_ADDR = 1;
    volatile int fd = *(int *)CTL_FD_ADDR;
    *c->fd = fd;
}


