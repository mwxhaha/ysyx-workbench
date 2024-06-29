#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
unsigned char data[N];
unsigned char data2[2 * N];

void reset()
{
    int i;
    for (i = 0; i < N; i++)
    {
        data[i] = '0' + i;
    }
    for (i = 0; i < 2 * N; i++)
    {
        data2[i] = '0' + i;
    }
}

void check_seq(int l, int r)
{
    int i;
    for (i = 0; i < r; i++)
    {
        assert(data2[i] == '0' + i);
    }
    for (i = r + N - l; i < 2 * N; i++)
    {
        assert(data2[i] == '0' + i);
    }
}

void check_eq(int l, int r)
{
    int i;
    for (i = r; i < r + N - l; i++)
    {
        assert(data[i - r + l] == data2[i]);
    }
}

void test_memcpy()
{
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = 0; r < N; r++)
        {
            reset();
            memcpy(data2 + r, data + l, N - l);
            check_seq(l, r);
            check_eq(l, r);
        }
    }
}

int main()
{
    test_memcpy();
    return 0;
}
