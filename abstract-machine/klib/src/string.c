#include <klib-macros.h>
#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

char *strcpy(char *dst, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = src[i];
    return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    panic("Not implemented");
}

char *strcat(char *dst, const char *src)
{
    int i = 0;
    while (dst[i] != '\0')
        i++;
    int j = 0;
    while (src[j] != '\0')
    {
        dst[i + j] = src[j];
        j++;
    }
    dst[i + j] = src[j];
    return dst;
}

int strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] < s2[i])
            return -1;
        if (s1[i] > s2[i])
            return 1;
        i++;
    }
    if (s1[i] < s2[i])
        return -1;
    if (s1[i] > s2[i])
        return 1;
    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        if (s1[i] < s2[i])
            return -1;
        if (s1[i] > s2[i])
            return 1;
    }
    return 0;
}

void *memset(void *s, int c, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        ((unsigned char *)s)[i] = (unsigned char)c;
    }
    return s;
}

#define MOVE_TMP_LEN_MAX 10000
static unsigned char move_tmp[MOVE_TMP_LEN_MAX];

void *memmove(void *dst, const void *src, size_t n)
{
    panic_on(n > MOVE_TMP_LEN_MAX, "memmove size is too large");
    for (int i = 0; i < n; i++)
        move_tmp[i] = ((unsigned char *)src)[i];
    for (int i = 0; i < n; i++)
        ((unsigned char *)dst)[i] = move_tmp[i];
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    for (int i = 0; i < n; i++)
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
    return dst;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    for (int i = 0; i < n; i++)
    {
        if (((unsigned char *)s1)[i] < ((unsigned char *)s2)[i])
            return -1;
        if (((unsigned char *)s1)[i] > ((unsigned char *)s2)[i])
            return 1;
    }
    return 0;
}

#endif
