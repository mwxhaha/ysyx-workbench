#include <am.h>
#include <npc.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init()
{
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg)
{
    int i;
    int w = inw(VGACTL_ADDR + 2); // TODO: get the correct width
    int h = inw(VGACTL_ADDR);     // TODO: get the correct height
    for (i = 0; i < w * h; i++)
    {
        outl(FB_ADDR + i * 4, 0);
    }
    outl(SYNC_ADDR, 1);
    *cfg = (AM_GPU_CONFIG_T){.present = true, .has_accel = false, .width = w, .height = h, .vmemsz = w * h * 4};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl)
{
    if (ctl->pixels)
    {
        int width = inw(VGACTL_ADDR + 2);
        for (int i = 0; i < ctl->w; i++)
            for (int j = 0; j < ctl->h; j++)
            {
                outl(FB_ADDR + ((ctl->y + j) * width + ctl->x + i) * 4, ((uint32_t *)(ctl->pixels))[j * ctl->w + i]);
            }
    }
    if (ctl->sync)
    {
        outl(SYNC_ADDR, 1);
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
