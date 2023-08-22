#include <stdio.h>

#define mu_suite_start()    
#define mu_assert(test, message) \
    if (!(test))                 \
    {                            \
        printf(message);   \
    }
#define mu_run_test(test) test()
#define RUN_TESTS(name) \
    int main()          \
    {                   \
        all_tests();    \
    }
