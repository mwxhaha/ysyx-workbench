#include <sim_tool.hpp>
#include <cpu_sdb.hpp>
#include <Vtop__Dpi.h>
#include <math.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static int is_batch_mode = false;

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
  // cpu_exec(-1);
  return 0;
}

static int cmd_q(const char *const args)
{
  if (args != NULL)
  {
    printf("q format error, using like this: q\n");
    return 0;
  }
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
      //   cpu_exec(number);
    }
    else
    {
      printf("si format error, using like this: si N\n");
    }
  }
  else
  {
    // cpu_exec(1);
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
        //   isa_reg_display();
        break;
      case 'w':
        //   printf_watchpoint();
        break;
      default:
        printf("info format error, using like this: info r/w\n");
        break;
      }
    }
    else
    {
      printf("info format error, using like this: info r/w\n");
    }
  }
  else
  {
    printf("info format error, using like this: info r/w\n");
  }
  return 0;
}

static int cmd_x(const char *const args)
{
  if (args)
  {
    int scan_len;
    int result = sscanf(args, "%d", &scan_len);
    if (result == 1)
    {
      if (scan_len <= 0)
      {
        printf("scan length must be larger than 0\n");
        return 0;
      }
      const char *const e = args + (int)log10(scan_len) + 2;
      bool success = true;
      // vaddr_t addr = expr(e, &success);
      vaddr_t addr = 0;
      if (!success)
        return 0;
      int len = sizeof(word_t);
      int column_cnt_display = 8;
      for (int i = 0; i < scan_len; i++)
      {
        word_t val;
        pmem_read(addr + i * len, (int *)&val);
        printf(FMT_WORD " ", val);
        if (i % column_cnt_display == column_cnt_display - 1)
          printf("\n");
      }
      if (scan_len % column_cnt_display != 0)
        printf("\n");
    }
    else
    {
      printf("x format error, using like this: x N EXPR\n");
    }
  }
  else
  {
    printf("x format error, using like this: x N EXPR\n");
  }
  return 0;
}

static int cmd_p(const char *const args)
{
  if (args)
  {
    bool success = true;
    // word_t val = expr(args, &success);
    word_t val = 0;
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
  if (args != NULL)
  {
    // new_wp(args);
  }
  else
  {
    printf("w format error, using like this: w EXPR\n");
  }
  return 0;
}

static int cmd_d(const char *const args)
{
  if (args != NULL)
  {
    int n;
    int result = sscanf(args, "%d", &n);
    if (result == 1)
    {
      // free_wp(n);
    }
    else
    {
      printf("w format error, using like this: d N\n");
    }
  }
  else
  {
    printf("w format error, using like this: d N\n");
  }
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
    {"info", "Display the state of register and information of watching point",
     cmd_info},
    {"x", "scan the memory", cmd_x},
    {"p", "print the value of expression", cmd_p},
    {"w", "set the watchpoint", cmd_w},
    {"d", "delete the watchpoint", cmd_d}

};

#define NR_CMD sizeof(cmd_table) / sizeof(cmd_table[0])

static int cmd_help(const char *const args)
{
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
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

    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

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
  // init_regex();
  // init_wp_pool();
}
