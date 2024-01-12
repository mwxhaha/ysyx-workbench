#include <monitor/cpu_sdb.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <cpu/cpu_cpu_exec.hpp>
#include <cpu/cpu_ftrace.hpp>
#include <cpu/cpu_iringbuf.hpp>
#include <cpu/cpu_mem.hpp>
#include <cpu/cpu_reg.hpp>
#include <monitor/cpu_expr.hpp>
#include <monitor/cpu_watchpoint.hpp>

static int is_batch_mode = false;

/* We use the `readline' library to provide more flexibility to read from stdin.
 */
static char *rl_gets()
{
    static char *line_read = NULL;

    if (line_read)
    {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(npc) ");

    if (line_read && *line_read)
    {
        add_history(line_read);
    }

    return line_read;
}

static int cmd_c(const char *const args)
{
    if (args != NULL)
    {
        printf("c format error, using like this: c\n");
        return 0;
    }
    cpu_exec(-1);
    return 0;
}

static int cmd_q(const char *const args)
{
    if (args != NULL)
    {
        printf("q format error, using like this: q\n");
        return 0;
    }
    npc_state.state = NPC_QUIT;
    return -1;
}

static int cmd_si(const char *const args)
{
    if (args)
    {
        uint64_t number;
        int result = sscanf(args, "%ld", &number);
        if (result == 1)
        {
            if (number <= 0)
            {
                printf("step number must be larger than 0\n");
                return 0;
            }
            cpu_exec(number);
        }
        else
        {
            printf("si format error, using like this: si N\n");
        }
    }
    else
    {
        cpu_exec(1);
    }
    return 0;
}

static int cmd_info(const char *const args)
{
    if (args)
    {
        char cmd;
        int result = sscanf(args, "%c", &cmd);
        if (result == 1)
        {
            switch (cmd)
            {
            case 'r':
                isa_reg_display();
                break;
            case 'i':
                MUXDEF(CONFIG_ITRACE, print_iringbuf(), printf("itrace is close"));
                break;
            case 'm':
                MUXDEF(CONFIG_MTRACE, print_mtrace(), printf("mtrace is close"));
                break;
            case 'f':
                MUXDEF(CONFIG_FTRACE, print_ftrace(), printf("ftrace is close"));
                break;
            case 'w':
                MUXDEF(CONFIG_WATCHPOINT, printf_watchpoint(), printf("watchpoint is close\n"));
                break;
            default:
                printf("info format error, using like this: info r/i/m/f/d/w\n");
                break;
            }
        }
        else
        {
            printf("info format error, using like this: info r/i/m/f/d/w\n");
        }
    }
    else
    {
        printf("info format error, using like this: info r/i/m/f/d/w\n");
    }
    return 0;
}

#define COLUMN 32

static int cmd_x(const char *const args)
{
    if (args)
    {
        int scan_len, scan_num;
        int result = sscanf(args, "%d %d", &scan_len, &scan_num);
        if (result == 2)
        {
#if (ISA_WIDTH == 64)
            if (scan_len != 1 && scan_len != 2 && scan_len != 4 && scan_len != 8)
            {
                printf("scan length must be 1/2/4/8\n");
                return 0;
            }
#else
            if (scan_len != 1 && scan_len != 2 && scan_len != 4)
            {
                printf("scan length must be 1/2/4\n");
                return 0;
            }
#endif
            if (scan_num <= 0)
            {
                printf("scan number must be larger than 0\n");
                return 0;
            }

            const char *const e = args + 2 + (int)log10(scan_num) + 2;
            bool success = true;
            vaddr_t addr = expr(e, &success);
            if (!success)
                return 0;
            for (int i = 0; i < scan_num; i++)
            {
                IFDEF(CONFIG_MTRACE, disable_mtrace_once());
                word_t data = pmem_read(addr + i * scan_len, scan_len);
                switch (scan_len)
                {
                case 1:
                    printf("0x%02x ", data);
                    break;
                case 2:
                    printf("0x%04x ", data);
                    break;
                case 4:
                    printf("0x%08x ", data);
                    break;
#ifdef CONFIG_ISA64
                case 8:
                    printf("0x%016lx ", data);
                    break;
#endif
                }
                if ((i + 1) * scan_len % COLUMN == 0)
                    printf("\n");
            }
            if (scan_num * scan_len % COLUMN != 0)
                printf("\n");
        }
        else
        {
            printf("x format error, using like this: x len n EXPR\n");
        }
    }
    else
    {
        printf("x format error, using like this: x len n EXPR\n");
    }
    return 0;
}

static int cmd_p(const char *const args)
{
    if (args)
    {
        bool success = true;
        word_t val = expr(args, &success);
        if (!success)
            return 0;
        printf("%s = " FMT_WORD_T "\n", args, val);
    }
    else
    {
        printf("p format error, using like this: p EXPR\n");
    }
    return 0;
}

static int cmd_w(const char *const args)
{
#ifdef CONFIG_WATCHPOINT
    if (args != NULL)
    {
        int hit_cnt;
        int result = sscanf(args, "%d", &hit_cnt);
        if (result == 1)
        {
            new_wp(hit_cnt, args + (int)log10(hit_cnt) + 2);
        }
        else
        {
            printf("w format error, using like this: w n EXPR\n");
        }
    }
    else
    {
        printf("w format error, using like this: w n EXPR\n");
    }
#else
    printf("watchpoint is close\n");
#endif
    return 0;
}

static int cmd_d(const char *const args)
{
#ifdef CONFIG_WATCHPOINT
    if (args != NULL)
    {
        int n;
        int result = sscanf(args, "%d", &n);
        if (result == 1)
        {
            free_wp(n);
        }
        else
        {
            printf("d format error, using like this: d N\n");
        }
    }
    else
    {
        printf("d format error, using like this: d N\n");
    }
#else
    printf("watchpoint is close\n");
#endif
    return 0;
}

static int cmd_help(const char *const args);

static struct
{
    const char *name;
    const char *description;
    int (*handler)(const char *const);
} cmd_table[] = {
    {"help", "Display information about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Step in N instruction", cmd_si},
    {"info", "Display the state of register and information of itrace, mtrace, ftrace, dtrcae, watching point", cmd_info},
    {"x", "scan the memory", cmd_x},
    {"p", "print the value of expression", cmd_p},
    {"w", "set the watchpoint", cmd_w},
    {"d", "delete the watchpoint", cmd_d}

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(const char *const args)
{
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL)
    {
        /* no argument given */
        for (i = 0; i < NR_CMD; i++)
        {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else
    {
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(arg, cmd_table[i].name) == 0)
            {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

void sdb_set_batch_mode() { is_batch_mode = true; }

void sdb_mainloop()
{
    if (is_batch_mode)
    {
        cmd_c(NULL);
        return;
    }

    for (char *str; (str = rl_gets()) != NULL;)
    {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL)
        {
            continue;
        }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end)
        {
            args = NULL;
        }

        int i;
        for (i = 0; i < NR_CMD; i++)
        {
            if (strcmp(cmd, cmd_table[i].name) == 0)
            {
                if (cmd_table[i].handler(args) < 0)
                {
                    return;
                }
                break;
            }
        }

        if (i == NR_CMD)
        {
            printf("Unknown command '%s'\n", cmd);
        }
    }
}

void init_sdb()
{
    /* Compile the regular expressions. */
    init_regex();

    /* Initialize the watchpoint pool. */
    init_wp_pool();
}
