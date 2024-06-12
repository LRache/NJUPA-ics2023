#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buffer[1024];
  va_list ap;
  va_start(ap, fmt);
  int r = vsprintf(buffer, fmt, ap);
  va_end(ap);
  char *c = buffer;
  while (*c)
  {
    putch(*c++);
  }
  return r;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *p = (char*) fmt;
  char *ori = out;
  while (*p)
  {
    if (*p == '%') {
      p++;
      if (*p == '%') {
        *(out++) = *(p++);
      } else if (*p == 's') {
        char *s = va_arg(ap, char*);
        while (*s) *(out++) = *(s++);
        p++;
      } else if (*p == 'd') {
        int d = va_arg(ap, int);
        
        if (d == 0) {
          *(out++) = '0';
        } else {
          int sign = d < 0;
          if (sign) d = -d;
          char stack[13] = {};
          char *t = stack;
          while (d) {
            *(t++) = d % 10 + '0';
            d = d / 10;
          }
          char *h = stack;
          if (sign) (*out++) = '-';
          while (t > h) *(out++) = *(--t);
        }
        p++;
      }
    } else {
      *(out++) = *(p++);
    }
  }
  *out = 0;
  return out - ori - 1;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int result = vsprintf(out, fmt, ap);
  va_end(ap);
  return result;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
