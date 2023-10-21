#ifndef DEBUG_HPP
#define DEBUF_HPP

#include <cstdio>
#include <cassert>

#ifdef SIM_ALL
#define Assert(cond, format, ...)               \
    do                                          \
    {                                           \
        if (!(cond))                            \
        {                                       \
            printf(format "\n", ##__VA_ARGS__); \
            extern void assert_fail_msg();      \
            assert_fail_msg();                  \
            assert(cond);                       \
        }                                       \
    } while (0)
#else
#define Assert(cond, format, ...)               \
    do                                          \
    {                                           \
        if (!(cond))                            \
        {                                       \
            printf(format "\n", ##__VA_ARGS__); \
            assert(cond);                       \
        }                                       \
    } while (0)
#endif

#define panic(format, ...) Assert(0, format, ##__VA_ARGS__)

#endif
