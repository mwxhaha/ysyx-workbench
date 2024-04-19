#ifndef UTIL_LOG_HPP
#define UTIL_LOG_HPP

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <cpu_exec/cpu_exec.hpp>

#define ANSI_FG_BLACK "\33[1;30m"
#define ANSI_FG_RED "\33[1;31m"
#define ANSI_FG_GREEN "\33[1;32m"
#define ANSI_FG_YELLOW "\33[1;33m"
#define ANSI_FG_BLUE "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN "\33[1;36m"
#define ANSI_FG_WHITE "\33[1;37m"
#define ANSI_BG_BLACK "\33[1;40m"
#define ANSI_BG_RED "\33[1;41m"
#define ANSI_BG_GREEN "\33[1;42m"
#define ANSI_BG_YELLOW "\33[1;43m"
#define ANSI_BG_BLUE "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;35m"
#define ANSI_BG_CYAN "\33[1;46m"
#define ANSI_BG_WHITE "\33[1;47m"
#define ANSI_NONE "\33[0m"

#define ANSI_FMT(str, fmt) fmt str ANSI_NONE

extern FILE *log_fp;

#ifdef SIM_ALL

#define Assert(cond, format, ...)                                               \
    do                                                                          \
    {                                                                           \
        if (!(cond))                                                            \
        {                                                                       \
            fflush(stdout);                                                     \
            fprintf(stderr, ANSI_FMT(format, ANSI_FG_RED) "\n", ##__VA_ARGS__); \
            fflush(log_fp);                                                     \
            assert_fail_msg();                                                  \
            assert(cond);                                                       \
        }                                                                       \
    } while (0)

#else

#define Assert(cond, format, ...)                                      \
    do                                                                 \
    {                                                                  \
        if (!(cond))                                                   \
        {                                                              \
            printf(ANSI_FMT(format, ANSI_FG_RED) "\n", ##__VA_ARGS__); \
            assert(cond);                                              \
        }                                                              \
    } while (0)

#endif

#define panic(format, ...) Assert(0, format, ##__VA_ARGS__)

#define TODO() panic("please implement me")

void init_log(const char *log_file);
bool log_enable();

#define log_write(...)                    \
    do                                    \
    {                                     \
        if (log_enable())                 \
        {                                 \
            fprintf(log_fp, __VA_ARGS__); \
            fflush(log_fp);               \
        }                                 \
    } while (0)

#define _Log(...)               \
    do                          \
    {                           \
        printf(__VA_ARGS__);    \
        log_write(__VA_ARGS__); \
    } while (0)

#define Log(format, ...) \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#endif
