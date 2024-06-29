#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#define N 32
char data[N];
char data2[2 * N];

void reset()
{
    int i;
    for (i = 0; i < N - 1; i++)
    {
        data[i] = '0' + i;
    }
    data[N - 1] = '\0';
    for (i = 0; i < 2 * N - 1; i++)
    {
        data2[i] = '0' + i;
    }
    data2[N - 1] = '\0';
    data2[2 * N - 1] = '\0';
}

void check_seq(int l, int r)
{
    int i;
    for (i = 0; i < N - 1; i++)
    {
        assert(data2[i] == '0' + i);
    }
    for (i = 2 * N - l - 1; i < 2 * N - 1; i++)
    {
        assert(data2[i] == '0' + i);
    }
    assert(data2[2 * N - 1] == '\0');
}

void check_eq(int l, int r)
{
    int i;
    for (i = N - 1; i < 2 * N - l - 1; i++)
    {
        assert(data[i - N + 1 + l] == data2[i]);
    }
}

void test_strcat()
{
    int l, r;
    for (l = 0; l < N; l++)
    {
        for (r = 0; r < N; r++)
        {
            reset();
            strcat(data2 + r, data + l);
            check_seq(l, r);
            check_eq(l, r);
        }
    }
}

int main()
{
    test_strcat();
    return 0;
}
