#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
unsigned char data[N];

void reset()
{
    int i;
    for (i = 0; i < N; i++)
    {
        data[i] = i + 1;
    }
}

int max(int a, int b) { return a > b ? a : b; }

void test_memcmp()
{
    reset();
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = 0; r < N; r++)
        {
            int ans = memcmp(data + l, data + r, N - max(l, r));
            if (l < r)
                assert(ans < 0);
            else if (l > r)
                assert(ans > 0);
            else
                assert(ans == 0);
            ans = strncmp((char *)data + l, (char *)data + r, N - max(l, r));
            if (l < r)
                assert(ans < 0);
            else if (l > r)
                assert(ans > 0);
            else
                assert(ans == 0);
        }
    }
}

int main()
{
    test_memcmp();
    return 0;
}