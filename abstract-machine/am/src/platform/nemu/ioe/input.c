#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  volatile uint32_t *ptr = (uint32_t*)KBD_ADDR;
  volatile uint32_t key = *ptr;

  kbd->keydown = (key & KEYDOWN_MASK) != 0;
  kbd->keycode = (key & (KEYDOWN_MASK-1));
}
