#include <common.h>
#include <klib.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

static int canvas_width = 0;
static int canvas_height = 0;
static int canvas_x = 0;
static int canvas_y = 0;
static int canvas_ready = 0;

size_t serial_write(const void *buf, size_t offset, size_t len) {
  const char *buffer = buf;
  for (int i = 0; i < len; i++) {
    putch(buffer[i]);
  }
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  if (canvas_ready) {
    const char *s = "mmap ok";
    size_t l = strlen(s);
    size_t p = 0;
    for (; p < l && p < len; p++) ((char*)buf)[p] = s[p];
    canvas_ready = false;
    return p;
  }

  AM_INPUT_KEYBRD_T e = io_read(AM_INPUT_KEYBRD);
  if (e.keycode == 0) return 0;

  char buffer[3] = {[0]= 'k', [2] = ' '};
  if (e.keydown) buffer[1] = 'd';
  else buffer[1] = 'u';

  size_t p = 0;
  for (int i = 0; i < 3 && p < len; i++, p++) ((char*)buf)[p] = buffer[i];

  size_t l = strlen(keyname[e.keycode]);
  for (int i = 0; i < l && p < len; i++, p++) ((char*)buf)[p] = keyname[e.keycode][i];
  if (p < len) ((char*)buf)[p++] = '\n';

  return p;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T config = io_read(AM_GPU_CONFIG);
  char buffer[32];
  sprintf(buffer, "%d %d", config.width, config.height);
  int p = 0;
  for (; p < len && buffer[p] != '\0'; p++) ((char*)buf)[p] = buffer[p];
  return p;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x = (offset / 4) % canvas_width;
  int y = (offset / 4) / canvas_width;
  int count = len / 4;
  Log("%u %d %d", offset, x, y);
  int i = 0;
  for (; i < count; i++) {
    io_write(AM_GPU_FBDRAW, canvas_x + x, canvas_y + y, ((uint32_t*)buf+i), 1, 1, false);
    x ++;
    y += (x == canvas_width);
    x %= canvas_width;
  }
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  return i * 4;
}

size_t fbctl_write(const void *buf, size_t offset, size_t len) {
  int width = 0;
  int height = 0;
  int i = 0;
  const char *s = buf;
  for (; i < len && s[i] != ' '; i++) {
    width = width * 10 + s[i] - '0';
  }
  i++;
  for (; i < len && s[i] != '\0'; i++) {
    height = height * 10 + s[i] - '0';
  }
  canvas_width = width;
  canvas_height = height;
  canvas_ready = true;
  return i;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
