#include <am.h>
#include <nemu.h>
#include "klib.h"

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  //volatile uint64_t a = ((uint64_t)((*((uint32_t*)RTC_ADDR))+1) << 32);
  volatile uint32_t *ptr = (uint32_t*)RTC_ADDR;
  volatile uint32_t v = *ptr;
  printf("%d\n", v);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
