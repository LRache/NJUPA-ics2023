#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static void __fmt_s(char **out, va_list ap);
static void __fmt_d(char **out, va_list ap);
static void __fmt_llu(char **out, va_list ap);
static void __fmt_avoid(char **out, va_list ap);

typedef struct {
  char fmt[10];
  int length;
  void (*fun)(char**, va_list);
} FmtEntry;

static FmtEntry fmtTable[] = {
  {"s"   , 1, __fmt_s},
  {"d"   , 1, __fmt_d},
  {"llu" , 3, __fmt_llu},
  {"%"   , 1, __fmt_avoid}
};

#define FMT_TABLE_LEN (sizeof(fmtTable) / sizeof(fmtTable[0]))

static void __fmt_s(char **out, va_list ap) {
  char *s = va_arg(ap, char*);
  while (*s) *((*out)++) = *(s++);
}

static void __fmt_d(char **out, va_list ap) {
  int d = va_arg(ap, int);
        
  if (d == 0) {
    *((*out)++) = '0';
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
    if (sign) (*(*out)++) = '-';
    while (t > h) *((*out)++) = *(--t);
  }
}

static void __fmt_llu(char **out, va_list ap) {
  unsigned long long d = va_arg(ap, unsigned long long);
  if (d == 0) {
    *(*out) = '0';
    (*out)++;
  } else {
    char stack[21] = {};
    char *t = stack;
    while (d) {
      *(t++) = d % 10 + '0';
      d = d / 10;
    }
    char *h = stack;
    while (t > h) *((*out)++) = *(--t);
  }
}

static void __fmt_avoid(char **out, va_list ap) {
  *(*out)++ = '%';
}

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
      for (int i = 0; i < FMT_TABLE_LEN; i++) {
        putch(fmtTable[i].fmt[0]);
        putch('\n');
        if (strncmp(p, fmtTable[i].fmt, fmtTable[i].length) == 0) {
          fmtTable[i].fun(&out, ap);
          p += fmtTable[i].length;
          break;
        } 
      }
    }
    else {
      *(out++) = *(p++);
    }
  }
  *out = 0;
  return out - ori;
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
