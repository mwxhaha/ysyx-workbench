#ifndef CPU_LOG_HPP
#define CPU_LOG_HPP

#include <cstdio>
#include <cstdbool>
#include <util/debug.hpp>

extern FILE *log_fp;
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
