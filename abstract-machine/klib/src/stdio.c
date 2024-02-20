#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char num_to_char(int num)
{
    panic_on(num > 16, "num_to_char base error\n");
    if (num <= 9)
        return '0' + num;
    else
        return 'a' + num - 10;
}

#define NUM_STR_MAX 20

static int num_to_str(uint64_t num, char *num_str, int is_signed, int base)
{
    panic_on(base > 16, "num_to_str base error\n");
    if (num == 0)
    {
        num_str[0] = '0';
        return 1;
    }
    int num_str_len = 0;
    if (is_signed)
    {
        if (num == -0x7fffffffffffffffLL - 1LL)
        {
            num_str[0] = '-';
            num_str[1] = '9';
            num_str[2] = '2';
            num_str[3] = '2';
            num_str[4] = '3';
            num_str[5] = '3';
            num_str[6] = '7';
            num_str[7] = '2';
            num_str[8] = '0';
            num_str[9] = '3';
            num_str[10] = '6';
            num_str[11] = '8';
            num_str[12] = '5';
            num_str[13] = '4';
            num_str[14] = '7';
            num_str[15] = '7';
            num_str[16] = '5';
            num_str[17] = '8';
            num_str[18] = '0';
            num_str[19] = '8';
            return 20;
        }
        if ((int64_t)num < 0)
        {
            num_str[num_str_len] = '-';
            num_str_len++;
            num = -(int64_t)num;
        }
    }

    int i = 0;
    char num_str_inverse[NUM_STR_MAX];
    while (num != 0)
    {
        if (is_signed)
        {
            num_str_inverse[i] = num_to_char((int64_t)num % base);
            num = (int64_t)num / base;
        }
        else
        {
            num_str_inverse[i] = num_to_char(num % base);
            num = num / base;
        }
        i++;
        panic_on(i > NUM_STR_MAX, "num_to_str str len error\n");
    }
    i--;
    for (; i >= 0; i--)
    {
        num_str[num_str_len] = num_str_inverse[i];
        num_str_len++;
        panic_on(num_str_len > NUM_STR_MAX, "num_to_str str len error\n");
    }
    return num_str_len;
}

static uint64_t str_to_num(const char *num_str, int num_str_len)
{
    panic_on(num_str_len < 0, "str_to_num str len error\n");
    if (num_str_len == 0)
        return 0;
    uint64_t num = num_str[0] - '0';
    for (int i = 1; i < num_str_len; i++)
    {
        num = num * 10 + num_str[i] - '0';
        panic_on(num > 0x7fffffffffffffffLL * 2ULL + 1ULL, "str_to_num num is too large\n");
    }
    return num;
}

static char decode_fmt(const char *fmt, int *i, int *is_filling_zero, int *fmt_limit_len)
{
    (*i)++;
    int init_i = *i;
    *fmt_limit_len = 0;
    if (fmt[*i] == '0')
    {
        *is_filling_zero = 1;
        (*i)++;
    }
    else
    {
        *is_filling_zero = 0;
    }
    while (1)
    {
        if (fmt[*i] >= '0' && fmt[*i] <= '9')
        {
            (*i)++;
            continue;
        }
        if (fmt[*i] == 'd' || fmt[*i] == 'u' || fmt[*i] == 'x' || fmt[*i] == 'c' || fmt[*i] == 's')
        {
            *fmt_limit_len = str_to_num(fmt + init_i, *i - init_i);
            (*i)++;
            return fmt[*i - 1];
        }
        panic("not support fmt code");
    }
}

static void sprintf_limit_len(char *out, int *j, int fmt_limit_len, int num_str_len, int is_filling_zero, const char *num_str)
{
    while (fmt_limit_len > num_str_len)
    {
        if (is_filling_zero)
            out[*j] = '0';
        else
            out[*j] = ' ';
        fmt_limit_len--;
        (*j)++;
    }
    for (int i = 0; i < num_str_len; i++)
    {
        out[*j] = num_str[i];
        (*j)++;
    }
}

#define PRINTF_LEN_MAX 10000

int vsprintf(char *out, const char *fmt, va_list ap)
{
    int i = 0;
    int j = 0;
    while (fmt[i] != '\0')
    {
        if (fmt[i] == '%')
        {
            int is_filling_zero;
            int fmt_limit_len;
            char fmt_code = decode_fmt(fmt, &i, &is_filling_zero, &fmt_limit_len);
            switch (fmt_code)
            {
                char num_str[NUM_STR_MAX];
                int num_str_len;
            case 'd':
                int num = va_arg(ap, int);
                num_str_len = num_to_str(num, num_str, 1, 10);
                sprintf_limit_len(out, &j, fmt_limit_len, num_str_len, is_filling_zero, num_str);
                break;
            case 'u':
                unsigned int num1 = va_arg(ap, unsigned int);
                num_str_len = num_to_str(num1, num_str, 0, 10);
                sprintf_limit_len(out, &j, fmt_limit_len, num_str_len, is_filling_zero, num_str);
                break;
            case 'x':
                unsigned int num2 = va_arg(ap, unsigned int);
                num_str_len = num_to_str(num2, num_str, 0, 16);
                sprintf_limit_len(out, &j, fmt_limit_len, num_str_len, is_filling_zero, num_str);
                break;
            case 'c':
                char c = va_arg(ap, int);
                sprintf_limit_len(out, &j, fmt_limit_len, 1, 0, &c);
                break;
            case 's':
                char *s = va_arg(ap, char *);
                sprintf_limit_len(out, &j, fmt_limit_len, strlen(s), 0, s);
                break;
            default:
                panic("not support fmt code");
                break;
            }
        }
        else
        {
            out[j] = fmt[i];
            i++;
            j++;
            panic_on(j > PRINTF_LEN_MAX, "printf str is too long\n");
        }
    }
    out[j] = '\0';
    return j;
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    return vsprintf(out, fmt, ap);
    va_end(ap);
}

static char out[PRINTF_LEN_MAX];

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    int i = 0;
    while (out[i] != '\0')
    {
        putch(out[i]);
        i++;
    }
    return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
    panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
    panic("Not implemented");
}

#endif
