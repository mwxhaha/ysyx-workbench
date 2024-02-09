#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define PRINTF_LEN_MAX 1000
static char out[PRINTF_LEN_MAX];

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    int i = 0;
    while (out[i] != '\0')
    {
        putch(out[i]);
        i++;
        assert(i < PRINTF_LEN_MAX);
    }
    return ret;
}

char num_to_char(int num)
{
    assert(num <= 16);
    if (num <= 9)
        return '0' + num;
    else
        return 'a' + num - 10;
}

#define NMU_STR_MAX 25

static int num_to_str(uint64_t num, char *num_str, int is_signed, int base)
{
    assert(base <= 16);
    if (num == 0)
    {
        num_str[0] = '0';
        return 1;
    }
    int num_str_len = 0;
    if (is_signed)
    {
        if (num == (int64_t)(-0x7fffffffffffffffLL - 1LL))
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
    char num_str_inverse[NMU_STR_MAX];
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
            ;
            num = num / base;
        }
        i++;
        assert(i < NMU_STR_MAX);
    }
    i--;
    for (; i >= 0; i--)
    {
        num_str[num_str_len] = num_str_inverse[i];
        num_str_len++;
        assert(num_str_len < NMU_STR_MAX);
    }
    return num_str_len;
}

static int str_to_num(const char *num_str, int num_str_len)
{
    assert(num_str_len >= 0);
    if (num_str_len == 0)
        return 0;
    int num = num_str[0] - '0';
    for (int i = 1; i < num_str_len; i++)
    {
        num = num * 10 + num_str[i] - '0';
        assert(num < 1000);
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

void sprintf_limit_len(char *out, int *j, int fmt_limit_len, int num_str_len, int is_filling_zero, const char *num_str)
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
                char num_str[NMU_STR_MAX];
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
        }
    }
    out[j] = '\0';
    va_end(ap);
    return j;
}

int sprintf(char *out, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    return vsprintf(out, fmt, ap);
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
