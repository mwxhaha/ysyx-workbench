#ifndef LOG_HPP
#define LOG_HPP

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <sim/cpu.hpp>
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

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
