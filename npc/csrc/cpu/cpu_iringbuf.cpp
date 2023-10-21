#include <cpu/cpu_iringbuf.hpp>

#include <cstdbool>
#include <cstdio>
#include <cstring>

#include <sim/cpu_sim.hpp>

#define IRINGBUF_MAX 100

static int iringbuf_ptr = 0;
static bool iringbuf_full = false;
static char iringbuf[IRINGBUF_MAX][128];

void add_iringbuf(const char *inst)
{
    strcpy(iringbuf[iringbuf_ptr], inst);
    iringbuf_ptr++;
    if (iringbuf_ptr == IRINGBUF_MAX)
    {
        iringbuf_ptr = 0;
        iringbuf_full = true;
    }
}

void print_iringbuf()
{
    if (iringbuf_full)
    {
        int i = iringbuf_ptr;
        puts(iringbuf[i]);
        i++;
        if (i == IRINGBUF_MAX)
            i = 0;
        while (i != iringbuf_ptr)
        {
            puts(iringbuf[i]);
            i++;
            if (i == IRINGBUF_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        while (i != iringbuf_ptr)
        {
            puts(iringbuf[i]);
            i++;
        }
    }
}
