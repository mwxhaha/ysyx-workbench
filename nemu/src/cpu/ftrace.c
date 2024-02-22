#include <common.h>
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <elf.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef CONFIG_ISA64
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
typedef Elf64_Sym Elf_Sym;
#else
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
typedef Elf32_Sym Elf_Sym;
#endif

#define SECTION_HEADERS_MAX 100
static Elf_Shdr section_headers[SECTION_HEADERS_MAX];

#define SYM_NAME_MAX 100
#define SYM_TAB_MAX 10000
static Elf_Sym sym_tab[SYM_TAB_MAX];
static char shstr[SYM_NAME_MAX];

#define FUNC_NAME_MAX 100
#define FUNC_INFOS_MAX 10000
typedef struct
{
    vaddr_t start_addr;
    vaddr_t end_addr;
    char func_name[FUNC_NAME_MAX];
} func_info_t;
static func_info_t func_infos[FUNC_INFOS_MAX];
static int func_infos_max = 0;

void load_elf(const char *elf_file)
{
#if (ISA_WIDTH == 64)
    panic("do not support isa 64");
#endif

    if (elf_file == NULL)
    {
        Log("No elf is given. ftrace will not work");
        ftrace_enable = false;
        return;
    }

    FILE *fp = fopen(elf_file, "rb");
    Assert(fp, "Can not open '%s'", elf_file);

    Elf_Ehdr elf_header;
    int ret = fread(&elf_header, sizeof(Elf_Ehdr), 1, fp);
    Assert(ret == 1, "elf_header read failed");

    fseek(fp, elf_header.e_shoff, SEEK_SET);
    Assert(elf_header.e_shnum <= SECTION_HEADERS_MAX, "too many sections");
    ret = fread(&section_headers, sizeof(Elf_Shdr), elf_header.e_shnum, fp);
    Assert(ret == elf_header.e_shnum, "section_headers read failed");

    int sym_tab_max = 0;
    int e_strndx = 0;
    for (int i = 0; i < elf_header.e_shnum; i++)
    {
        fseek(fp,
              section_headers[elf_header.e_shstrndx].sh_offset +
                  section_headers[i].sh_name,
              SEEK_SET);
        char *ret1 = fgetstr(shstr, SYM_NAME_MAX, fp);
        Assert(ret1, "shstr read failed");

        if (strcmp(shstr, ".symtab") == 0)
        {
            sym_tab_max = section_headers[i].sh_size / sizeof(Elf_Sym);
            Assert(sym_tab_max < SYM_TAB_MAX, "too many symbols");
            fseek(fp, section_headers[i].sh_offset, SEEK_SET);
            ret = fread(sym_tab, sizeof(Elf_Sym), sym_tab_max, fp);
            Assert(ret == sym_tab_max, "sym_tab read failed");
        }
        else if (strcmp(shstr, ".strtab") == 0)
            e_strndx = i;
    }

    for (int i = 0; i < sym_tab_max; i++)
    {
        if ((sym_tab[i].st_info & 0xf) == STT_FUNC)
        {
            func_infos[func_infos_max].start_addr = sym_tab[i].st_value;
            func_infos[func_infos_max].end_addr =
                sym_tab[i].st_value + sym_tab[i].st_size;
            fseek(fp, section_headers[e_strndx].sh_offset + sym_tab[i].st_name,
                  SEEK_SET);
            char *ret1 =
                fgetstr(func_infos[func_infos_max].func_name, SYM_NAME_MAX, fp);
            Assert(ret1, "func_name read failed");
            func_infos_max++;
            Assert(func_infos_max < FUNC_INFOS_MAX, "too many function symbols");
        }
    }

    fclose(fp);
}

typedef struct
{
    vaddr_t pc;
    int func_num;
    vaddr_t npc;
    int nfunc_num;
    int is_ret;
} ftrace_t;
#define FTRACE_ARRAY_MAX 20
static ftrace_t ftrace_array[FTRACE_ARRAY_MAX];
static int ftrace_array_end = 0;
static bool ftrace_array_is_full = false;
bool ftrace_enable = true;

void ftrace_record(Decode *s)
{
    int is_ret = 0;
    if (s->isa.inst.val == 0x00008067)
        is_ret = 1;
    int func_num = -1;
    int nfunc_num = -1;
    for (int i = 0; i < func_infos_max; i++)
    {
        if (func_infos[i].start_addr <= s->pc && func_infos[i].end_addr >= s->pc)
            func_num = i;
        if (is_ret == 0)
        {
            if (func_infos[i].start_addr == s->dnpc)
                nfunc_num = i;
        }
        else
        {
            if (func_infos[i].start_addr <= s->dnpc &&
                func_infos[i].end_addr >= s->dnpc)
                nfunc_num = i;
        }
    }
    if (func_num != -1 && nfunc_num != -1)
    {
        ftrace_array[ftrace_array_end].pc = s->pc;
        ftrace_array[ftrace_array_end].func_num = func_num;
        ftrace_array[ftrace_array_end].npc = s->dnpc;
        ftrace_array[ftrace_array_end].nfunc_num = nfunc_num;
        ftrace_array[ftrace_array_end].is_ret = is_ret;
        ftrace_array_end++;
        if (ftrace_array_end >= FTRACE_ARRAY_MAX)
        {
            ftrace_array_is_full = true;
            ftrace_array_end = 0;
        }
    }
}

static void print_ftrace_one(int i, int *func_stack)
{
    printf(FMT_WORD " -> " FMT_WORD ": ", ftrace_array[i].pc, ftrace_array[i].npc);
    if (ftrace_array[i].is_ret == 1)
        (*func_stack)--;
    for (int j = 0; j < *func_stack; j++)
        printf("| ");
    if (ftrace_array[i].is_ret == 0)
    {
        printf("call %s -> %s", func_infos[ftrace_array[i].func_num].func_name, func_infos[ftrace_array[i].nfunc_num].func_name);
        (*func_stack)++;
    }
    else
    {
        printf("ret  %s -> %s", func_infos[ftrace_array[i].func_num].func_name, func_infos[ftrace_array[i].nfunc_num].func_name);
    }
    printf("\n");
}

void print_ftrace()
{
    if (!ftrace_array_is_full && ftrace_array_end == 0)
    {
        printf("ftrace is empty now\n");
        return;
    }
    if (ftrace_array_is_full)
    {
        int i = ftrace_array_end;
        int func_stack = FTRACE_ARRAY_MAX;
        print_ftrace_one(i, &func_stack);
        i++;
        if (i == FTRACE_ARRAY_MAX)
            i = 0;
        while (i != ftrace_array_end)
        {
            print_ftrace_one(i, &func_stack);
            i++;
            if (i == FTRACE_ARRAY_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        int func_stack = FTRACE_ARRAY_MAX;
        while (i != ftrace_array_end)
        {
            print_ftrace_one(i, &func_stack);
            i++;
        }
    }
}
