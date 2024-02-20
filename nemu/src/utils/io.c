#include <stdio.h>
#include <stddef.h>

char *fgetstr(char *s, int n, FILE *stream)
{
    for (int i = 0; i < n; i++)
    {
        char c = fgetc(stream);
        if (c == EOF)
            return NULL;
        s[i] = c;
        if (c == '\0')
            break;
    }
    return s;
}
