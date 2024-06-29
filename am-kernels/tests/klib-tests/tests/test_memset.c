#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
unsigned char data[2 * N];

void reset()
{
    int i;
    for (i = 0; i < 2 * N; i++)
    {
        data[i] = i;
    }
}

void check_seq(int l, int r)
{
    int i;
    for (i = 0; i < l; i++)
    {
        assert(data[i] == i);
    }
    for (i = l + r; i < 2 * N; i++)
    {
        assert(data[i] == i);
    }
}

void check_eq(int l, int r, int val)
{
    int i;
    for (i = l; i < l + r; i++)
    {
        assert(data[i] == val);
    }
}

void test_memset()
{
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = 0; r < N; r++)
        {
            reset();
            unsigned char val = (l + r) / 2;
            memset(data + l, val, r);
            check_seq(l, r);
            check_eq(l, r, val);
        }
    }
}

int main()
{
    test_memset();
    return 0;
}
