#include <am.h>
#include <nemu.h>

#include "klib.h"

#define SYNC_ADDR (VGACTL_ADDR + 4)

static AM_GPU_CONFIG_T gpuConfig;

void __am_gpu_config(AM_GPU_CONFIG_T *cfg);

void __am_gpu_init() {
  __am_gpu_config(&gpuConfig);
  int i;
  int w = gpuConfig.width; 
  int h = gpuConfig.height;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t screenSize = inl(VGACTL_ADDR);
  int width = screenSize >> 16;
  int height = screenSize & 0x7fff;
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, .height = height,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // uint32_t *pixels = ctl->pixels;
  // uint32_t *buffer = (uint32_t*) FB_ADDR;
  // for (int i = 0; i < gpuConfig.width * gpuConfig.height; i++)
  // {
  //    *(buffer+i) = *(pixels+i);
  // }
  // if (ctl->sync) {
  //   outl(SYNC_ADDR, 1);
  // }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
