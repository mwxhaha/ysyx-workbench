#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
char data[N];

void reset()
{
    int i;
    for (i = 0; i < N; i++)
    {
        data[i] = i + 1;
    }
}

void test_strlen()
{
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = l; r < N; r++)
        {
            reset();
            data[r] = '\0';
            int ans = strlen(data + l);
            assert(ans == r - l);
        }
    }
}

int main()
{
    test_strlen();
    return 0;
}