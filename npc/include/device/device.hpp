#ifndef DEVICE_DEVICE_HPP
#define DEVICE_DEVICE_HPP

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
#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

#define TIMER_HZ 60

void device_update();
void sdl_clear_event_queue();
void init_device();
void device_quit();

#endif
