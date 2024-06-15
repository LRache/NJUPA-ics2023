#include <am.h>
#include <nemu.h>
#include "klib.h"

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

static uint32_t bufsize = 0;

void __am_audio_init() {
  bufsize = *(uint32_t*)AUDIO_SBUF_SIZE_ADDR;
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = bufsize;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  *(uint32_t*)AUDIO_FREQ_ADDR = ctrl->freq;
  *(uint32_t*)AUDIO_CHANNELS_ADDR = ctrl->channels;
  *(uint32_t*)AUDIO_SAMPLES_ADDR = ctrl->samples;
  *(uint32_t*)AUDIO_INIT_ADDR = 1;
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = *(uint32_t*)AUDIO_COUNT_ADDR;
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  uint32_t len = ctl->buf.end - ctl->buf.start;
  uint32_t left = bufsize - *(uint32_t*)AUDIO_COUNT_ADDR;
  printf("WAITING\n");
  while (left < len) left = bufsize - *(uint32_t*)AUDIO_COUNT_ADDR;
  printf("DONE\n");

  uint8_t *dst = (uint8_t*)AUDIO_SBUF_ADDR;
  uint8_t *src = (uint8_t*)ctl->buf.start;
  memcpy(dst, src, len);
}
