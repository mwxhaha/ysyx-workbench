#include <device/audio.hpp>

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
#include <device/map.hpp>
#include <device/mmio.hpp>
#include <SDL2/SDL.h>

#define CONFIG_AUDIO_CTL_MMIO 0xa0000200
#define CONFIG_SB_ADDR 0xa1200000
#define CONFIG_SB_SIZE 0x10000

enum
{
    reg_freq,
    reg_channels,
    reg_samples,
    reg_sbuf_size,
    reg_init,
    reg_count,
    nr_reg
};

#define REG_FREQ *(audio_base + reg_freq)
#define REG_CHANNELS *(audio_base + reg_channels)
#define REG_SAMPLES *(audio_base + reg_samples)
#define REG_SBUF_SIZE *(audio_base + reg_sbuf_size)
#define REG_INIT *(audio_base + reg_init)
#define REG_COUNT *(audio_base + reg_count)

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

static void audio_play(void *userdata, Uint8 *stream, int len)
{
    SDL_memset(stream, 0, len);
    int audio_len = REG_COUNT;
    if (audio_len == 0)
        return;
    len = (len > audio_len ? audio_len : len);
    SDL_MixAudio(stream, sbuf, len, SDL_MIX_MAXVOLUME);
    REG_COUNT = audio_len - len;
    for (int i = 0; i < audio_len - len; i++)
    {
        sbuf[i] = sbuf[i + len];
    }
}

static void audio_io_handler(uint32_t offset, int len, bool is_write)
{
    if (offset == sizeof(uint32_t) * reg_sbuf_size && !is_write)
    {
        REG_SBUF_SIZE = CONFIG_SB_SIZE;
        REG_COUNT = 0;
        REG_INIT = 0;
    }
    else if (offset == sizeof(uint32_t) * reg_init && is_write)
    {
        if (REG_INIT == 1)
        {
            SDL_AudioSpec s;
            SDL_memset(&s, 0, sizeof(s));
            s.freq = REG_FREQ;
            s.format = AUDIO_S16SYS;
            s.channels = REG_CHANNELS;
            s.samples = REG_SAMPLES;
            s.callback = audio_play;
            SDL_InitSubSystem(SDL_INIT_AUDIO);
            SDL_OpenAudio(&s, NULL);
            SDL_PauseAudio(0);
        }
    }
}

void init_audio()
{
    uint32_t space_size = sizeof(uint32_t) * nr_reg;
    audio_base = (uint32_t *)new_space(space_size);
    add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);

    sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
    add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
}
