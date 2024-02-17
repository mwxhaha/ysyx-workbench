#include <device/device.hpp>

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
#include <util/timer.hpp>
#include <device/map.hpp>
#include <device/serial.hpp>
#include <device/d_timer.hpp>
#include <device/keyboard.hpp>
#include <device/vga.hpp>
#include <device/audio.hpp>
#include <SDL2/SDL.h>

void device_update()
{
    static uint64_t last = 0;
    uint64_t now = get_time();
    if (now - last < 1000000 / TIMER_HZ)
    {
        return;
    }
    last = now;

    IFDEF(CONFIG_HAS_VGA, vga_update_screen());

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            npc_state.state = NPC_QUIT;
            break;
#ifdef CONFIG_HAS_KEYBOARD
        // If a key was pressed
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            uint8_t k = event.key.keysym.scancode;
            bool is_keydown = (event.key.type == SDL_KEYDOWN);
            send_key(k, is_keydown);
            break;
        }
#endif
        default:
            break;
        }
    }
}

void sdl_clear_event_queue()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        ;
}

void init_device()
{
    init_map();

    IFDEF(CONFIG_HAS_SERIAL, init_serial());
    IFDEF(CONFIG_HAS_TIMER, init_timer());
    IFDEF(CONFIG_HAS_VGA, init_vga());
    IFDEF(CONFIG_HAS_KEYBOARD, init_i8042());
    IFDEF(CONFIG_HAS_AUDIO, init_audio());
}

void device_quit()
{
    map_quit();
    IFDEF(CONFIG_HAS_VGA, vga_quit());
}
