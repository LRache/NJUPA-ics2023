#include <device/map.h>
#include <stdio.h>

static uint32_t *ctl;
static char *arg;
static char *buf;
static uint32_t ctl_space_size;

enum {
    reg_op,
    reg_fd,
    reg_arg,
    reg_excute,
    nr_reg
};

enum {
    LOCAL_OPEN,
    LOCAL_CLOSE,
    LOCAL_READ,
    LOCAL_WRITE,
    LOCAL_SEEK,
};

static FILE *files[32];

static void local_open() {
    int fd = 0;
    for (; fd < 32; fd++) {
        if (files[fd] == NULL) break;
    }
    if (fd == 32) {
        ctl[reg_fd] = -1;
        return;
    }
    char *mode;
    if (ctl[reg_arg] == 0) {
        mode = "r";
    } else {
        mode = "w";
    }

    FILE *f = fopen(arg, mode);
    if (f == NULL) {
        ctl[reg_fd] = -1;
    } else {
        files[fd] = f;
        ctl[reg_fd] = fd;
    }
}

static void local_close() {
    uint32_t fd = ctl[reg_fd];
    if (fd >= 32) {
        ctl[reg_arg] = -1;
        return ;
    }
    fclose(files[fd]);
}

static void local_ctl_handler(uint32_t offset, int len, bool is_write) {
    if (!is_write) return;
    if (offset == reg_excute) {
        uint32_t op = ctl[reg_op];
        switch (op)
        {
        case LOCAL_OPEN:    local_open();   break;
        case LOCAL_CLOSE:   local_close();  break;
        default:
            break;
        }
    }
}

static void local_arg_handler(uint32_t offset, int len, bool is_write) {

}

static void local_buf_handler(uint32_t offset, int len, bool is_write) {
    assert(!is_write);
}

void init_local() {
    ctl_space_size = sizeof(uint32_t) * nr_reg;
    ctl = (uint32_t*)new_space(ctl_space_size);
    arg = (char *)new_space(48);
    buf = (char *)new_space(1024);

    add_mmio_map("local disk ctl", CONFIG_LOCAL_CTL_MMIO, ctl, ctl_space_size, local_ctl_handler);
    add_mmio_map("local disk arg", CONFIG_LOCAL_ARG_MMIO, arg, 48, local_arg_handler);
    add_mmio_map("local disk buf", CONFIG_LOCAL_BUF_MMIO, buf, 1024, local_buf_handler);

}
