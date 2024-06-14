/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint8_t audio_buffer[CONFIG_SB_SIZE];
static uint32_t buffer_count = 0;
static uint32_t buf_head = 0;
static uint32_t buf_tail = 0;
static uint32_t *audio_base = NULL;

static SDL_AudioSpec s = {};

static void audio_callback(void *userdata, uint8_t *stream, int len) {
  if (buffer_count >= len) {
    for (int i = 0; i < len; i++) {
      stream[i] = audio_buffer[buf_head];
      buf_head = (buf_head + 1) % CONFIG_SB_SIZE;
    }
    buffer_count += len;
    Log("write %u", len);
    audio_base[reg_count] = buffer_count; 
  }
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  int reg = offset / 4;
  assert(reg < nr_reg);
  if (is_write) {
    assert(reg != reg_sbuf_size);
    if (reg == reg_init) {
      s.format = AUDIO_S16SYS;  // 假设系统中音频数据的格式总是使用16位有符号数来表示
      s.userdata = NULL;        // 不使用
      s.freq = *(audio_base + reg_freq);
      s.channels = *(audio_base + reg_channels);
      s.samples = *(audio_base + reg_samples);
      s.callback = audio_callback;
      SDL_InitSubSystem(SDL_INIT_AUDIO);
      SDL_OpenAudio(&s, NULL);
      SDL_PauseAudio(0);
    }
  }
}

static void audio_buf_io_handler(uint32_t offset, int len, bool is_write) {
  assert(is_write == 1);
  assert(len <= buffer_count);
  for (int i = 0; i < len; i++) {
    audio_buffer[buf_tail] = sbuf[offset+i];
    buf_tail = (buf_tail + 1) % CONFIG_SB_SIZE;
  }
  buffer_count -= len;
  audio_base[reg_count] = buffer_count;
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
  *(audio_base + reg_sbuf_size) = CONFIG_SB_SIZE;
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif
  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  buffer_count = CONFIG_SB_SIZE;
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, audio_buf_io_handler);
}
