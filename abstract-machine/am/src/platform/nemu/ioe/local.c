#include <am.h>
#include <nemu.h>
#include <klib.h>

#define CTL_OP_ADDR   (LOCAL_CTL_ADDR +  0)
#define CTL_FD_ADDR   (LOCAL_CTL_ADDR +  4)
#define CTL_ARG_ADDR  (LOCAL_CTL_ADDR +  8)
#define CTL_EXE_ADDR  (LOCAL_CTL_ADDR + 12)

#define CTL_OPEN    0
#define CTL_CLOSE   1
#define CTL_READ    2
#define CTL_WRITE   3
#define CTL_SEEK    4

#define LOCAL_BUF_SIZE 1024

static inline void __execute() {
     *(uint32_t *)CTL_EXE_ADDR = 1;
}

void __am_local_open(AM_LOCAL_OPEN_T *c) {
    *(uint32_t *)CTL_OP_ADDR = CTL_OPEN;
    *(uint32_t *)CTL_ARG_ADDR = c->mode;
    strcpy((char *)LOCAL_ARG_ADDR, c->pathname);
    __execute();
    volatile int fd = *(int *)CTL_FD_ADDR;
    *c->fd = fd;
}

void __am_local_close(AM_LOCAL_CLOSE_T *c) {
    *(uint32_t *)CTL_OP_ADDR = CTL_CLOSE;
    *(int *)CTL_FD_ADDR = c->fd;
    __execute();
    volatile int ret = *(int *)CTL_ARG_ADDR;
    *c->ret = ret;
}

void __am_local_read(AM_LOCAL_READ_T *c) {
    *(uint32_t *)CTL_OP_ADDR = CTL_READ;
    *(int *)CTL_FD_ADDR = c->fd;
    __execute();
    volatile int ret = *(int *)CTL_ARG_ADDR;
    *c->ret = ret;
    if (ret != -1) {
        memcpy((void *)LOCAL_BUF_ADDR, c->buffer, ret);
    }
}

void __am_local_write(AM_LOCAL_WRITE_T *c) {
    *(uint32_t *)CTL_OP_ADDR = CTL_WRITE;
    uint32_t length = c->length < LOCAL_BUF_SIZE ? c->length : LOCAL_BUF_SIZE;
    *(uint32_t *)CTL_ARG_ADDR = length;
    memcpy((void *)LOCAL_BUF_ADDR, (void *)c->buffer, length);
    __execute();
    volatile int ret = *(int *)CTL_ARG_ADDR;
    *c->ret = ret;
}

void __am_local_seek(AM_LOCAL_SEEK_T *c) {
    *(uint32_t *)CTL_OP_ADDR = CTL_SEEK;
    *(int *)CTL_ARG_ADDR = c->whence;
    *(int64_t *)LOCAL_BUF_ADDR = c->offset;
    __execute();
    volatile uint64_t ret = *(uint64_t *)LOCAL_BUF_ADDR;
    *c->ret = ret;
}
