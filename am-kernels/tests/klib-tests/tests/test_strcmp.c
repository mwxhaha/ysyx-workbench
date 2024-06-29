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

void test_strcmp()
{
    reset();
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = 0; r < N; r++)
        {
            int ans = strcmp(data + l, data + r);
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
    test_strcmp();
    return 0;
}