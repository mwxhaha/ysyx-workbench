#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) { panic("Not implemented"); }

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int i = 0;
  int j = 0;
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      switch (fmt[i + 1]) {
        case 'd':
          int d = va_arg(ap, int);
          char d_str[20];
          int k = 0;
          if (d==0) {
            out[j] = '0';
            j++;
            break;
          }
          if (d<0) {
            out[j] = '-';
            j++;
            d = -d;
          }
          while (d != 0) {
            d_str[k] = d % 10;
            k++;
            d = d / 10;
          }
          k--;
          while (k >= 0) {
            out[j] = '0'+d_str[k];
            k--;
            j++;
          }
          break;
        case 's':
          char *s = va_arg(ap, char *);
          int k1 = 0;
          while (s[k1] != '\0') {
            out[j] = s[k1];
            j++;
            k1++;
          }
          out[j] = s[k1];
          j++;
          break;
        default:
          panic("Not implemented");
          break;
      }
      i+=2;
    }
    else {
    out[j] = fmt[i];
    i++;
    j++;
    }
  }
  out[j] = fmt[i];
  va_end(ap);
  return j;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
