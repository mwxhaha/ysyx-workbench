#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char out[1000];

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = vsprintf(out, fmt, ap);
  int i = 0;
  while (out[i] != '\0') {
    putch(out[i]);
    i++;
  }
  return ret;
}

static void number_to_str(int d, char *d_str, int *index) {
  if (d == 0) {
    d_str[0] = '0';
    (*index)++;
    return;
  }
  if (d == -2147483648) {
    d_str[0] = '-';
    d_str[1] = '2';
    d_str[2] = '1';
    d_str[3] = '4';
    d_str[4] = '7';
    d_str[5] = '4';
    d_str[6] = '8';
    d_str[7] = '3';
    d_str[8] = '6';
    d_str[9] = '4';
    d_str[10] = '8';
    (*index)+=11;
    return;
  }
  if (d < 0) {
    d_str[0] = '-';
    (*index)++;
    d = -d;
  }
  char d_str_inverse[20];
  int i = 0;
  while (d != 0) {
    d_str_inverse[i] = '0' + d % 10;
    i++;
    d = d / 10;
  }
  i--;
  for (; i >= 0; i--) {
    d_str[(*index)] = d_str_inverse[i];
    (*index)++;
  }
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int i = 0;
  int j = 0;
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      switch (fmt[i + 1]) {
        case 'd':
          int d = va_arg(ap, int);
          number_to_str(d, out, &j);
          break;
        case 's':
          char *s = va_arg(ap, char *);
          int k = 0;
          while (s[k] != '\0') {
            out[j] = s[k];
            j++;
            k++;
          }
          break;
        default:
          panic("not support fmt code");
          break;
      }
      i += 2;
    } else {
      out[j] = fmt[i];
      i++;
      j++;
    }
  }
  out[j] = '\0';
  va_end(ap);
  return j;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  return vsprintf(out, fmt, ap);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
