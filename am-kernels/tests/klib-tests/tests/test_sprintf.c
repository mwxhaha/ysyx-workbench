#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <limits.h>
int data[] = {0, INT_MAX / 17, INT_MAX, INT_MIN, INT_MIN + 1,
              UINT_MAX / 17, INT_MAX / 17, UINT_MAX};
void test_sprintf()
{
    char out[100];
    sprintf(out, "%d", 0);
    assert(strcmp(out, "0") == 0);
    sprintf(out, "%d", INT_MAX - 1);
    assert(strcmp(out, "2147483646") == 0);
    sprintf(out, "%d", INT_MAX);
    assert(strcmp(out, "2147483647") == 0);
    sprintf(out, "%d", INT_MIN + 1);
    assert(strcmp(out, "-2147483647") == 0);
    sprintf(out, "%d", INT_MIN);
    assert(strcmp(out, "-2147483648") == 0);
    sprintf(out, "%s", "abc");
    assert(strcmp(out, "abc") == 0);
    sprintf(out, "%s", "");
    assert(strcmp(out, "") == 0);
    sprintf(out, "%s", "\0");
    assert(strcmp(out, "\0") == 0);
    sprintf(out, "%c", 'a');
    assert(strcmp(out, "a") == 0);
    sprintf(out, "%c", '\0');
    assert(strcmp(out, "\0") == 0);
    sprintf(out, "%s %d %c", "aaa", 1,'a');
    assert(strcmp(out, "aaa 1 a") == 0);
    sprintf(out, "aaa%sbbb%dccc%cddd%deee%sfff%dggg\n", "zzz", 987,'y',654, "xxx", 321);
    assert(strcmp(out, "aaazzzbbb987cccyddd654eeexxxfff321ggg\n") == 0);
    sprintf(out, "%5s %5d %05d", "aaa", 123, 456);
    assert(strcmp(out, "  aaa   123 00456") == 0);
    sprintf(out, "%5s %5u %05u", "aaa", 123, 456);
    assert(strcmp(out, "  aaa   123 00456") == 0);
    sprintf(out, "%5s %5x %05x", "aaa", 123, 456);
    assert(strcmp(out, "  aaa    7b 001c8") == 0);
}

int main()
{
    test_sprintf();
    return 0;
}
