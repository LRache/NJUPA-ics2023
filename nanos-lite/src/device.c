#include <common.h>

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

size_t serial_write(const void *buf, size_t offset, size_t len) {
  const char *buffer = buf;
  for (int i = 0; i < len; i++) {
    putch(buffer[i]);
  }
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T e = io_read(AM_INPUT_KEYBRD);
  if (e.keycode == 0) return 0;

  char buffer[3] = {[0]= 'k', [2] = ' '};
  if (e.keydown) buffer[1] = 'd';
  else buffer[1] = 'u';

  size_t p = 0;
  for (int i = 0; i < 3 && p < len; i++, p++) ((char*)buf)[p] = buffer[i];
  Log("%d %d", e.keycode, p);

  Log("%d %s", e.keycode, keyname[e.keycode]);
  size_t l = strlen(keyname[e.keycode]);
  for (int i = 0; i < l && p < len; i++, p++) ((char*)buf)[p] = keyname[e.keycode][i];
  if (p < len) ((char*)buf)[p++] = '\n';

  printf("%u\n", p);
  return p;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
