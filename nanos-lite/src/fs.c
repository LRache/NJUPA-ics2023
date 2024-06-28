#include <fs.h>

size_t ramdisk_read (void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define FILE_NUM (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < FILE_NUM; i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      file_table[i].open_offset = 0;
      return i;
    }
  }
  assert(0);
  return -1;
}

size_t fs_read(int fd, void *buf, size_t len) {
  if (fd == 1 || fd == 2) {
    assert(0);
    return 0;
  }
  Finfo info = file_table[fd];
  size_t left = info.size - info.open_offset;
  if (left == 0) {
    return 0;
  }
  size_t read_length = left > len ? len : left;
  ramdisk_read(buf, info.disk_offset + info.open_offset, read_length);
  file_table[fd].open_offset += read_length;
  return read_length;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  Finfo info = file_table[fd];
  size_t left = info.size - info.open_offset;
  if (left == 0) {
    return 0;
  }
  size_t write_length = left > len ? len : left;
  ramdisk_write(buf, info.disk_offset + info.open_offset, write_length);
  file_table[fd].open_offset += write_length;
  return write_length;
}

size_t fs_lseek(int fd, off_t offset, int whence) {
  size_t new_off = 0;
  switch (whence)
  {
  case SEEK_SET: new_off = offset; break;
  case SEEK_CUR: new_off = file_table[fd].open_offset + offset; break;
  case SEEK_END: new_off = file_table[fd].size + offset;
  default:
    return -1;
  }
  if (new_off >= 0 && new_off < file_table[fd].size) {
    file_table[fd].open_offset = new_off;
    return new_off;
  } else {
    return -1;
  }
}

int fs_close(int fd) {
  return 0;
}
