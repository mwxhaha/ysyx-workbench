#include <util/io.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

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
