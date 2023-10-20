#include <sdb/cpu_ftrace.hpp>

#include <elf.h>
#include <cstdio>
#include <cstdbool>
#include <cstring>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>
#include <util/io.hpp>
#include <cpu_cpu_exec.hpp>

#ifdef CONFIG_RV64
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
#define SYM_TAB_MAX 100
static Elf_Sym sym_tab[SYM_TAB_MAX];
static char shstr[SYM_NAME_MAX];

#define FUNC_NAME_MAX 100
#define FUNC_INFOS_MAX 100
typedef struct
{
    vaddr_t start_addr;
    vaddr_t end_addr;
    char func_name[FUNC_NAME_MAX];
} func_info_t;
static func_info_t func_infos[FUNC_INFOS_MAX];
static int func_infos_max = 0;
static bool open_ftrace = true;

bool load_elf(int argc, char **argv)
{
    char *elf_file=NULL;
    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], "-e") == 0)
        {
            elf_file = argv[i + 1];
            printf("use elf: %s\n",elf_file);
            break;
        }
    if (elf_file == NULL)
    {
        printf("No elf is given. ftrace will not work.\n");
        open_ftrace = false;
        return false;
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
        }
    }

    fclose(fp);
    return true;
}

typedef struct
{
    vaddr_t pc;
    int func_num;
    vaddr_t npc;
    int nfunc_num;
    int call_or_ret;
} ftrace_t;
#define FTRACES_MAX 1000
ftrace_t ftraces[FTRACES_MAX];
int ftraces_max = 0;

void ftrace_record(Decode *s)
{
    if (!open_ftrace)
        return;
    int call_or_ret = 0;
    if (s->isa.inst.val == 0x00008067)
        call_or_ret = 1;
    int func_num = -1;
    int nfunc_num = -1;
    for (int i = 0; i < func_infos_max; i++)
    {
        if (func_infos[i].start_addr <= s->pc && func_infos[i].end_addr >= s->pc)
            func_num = i;
        if (call_or_ret == 0)
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
        ftraces[ftraces_max].pc = s->pc;
        ftraces[ftraces_max].func_num = func_num;
        ftraces[ftraces_max].npc = s->dnpc;
        ftraces[ftraces_max].nfunc_num = nfunc_num;
        ftraces[ftraces_max].call_or_ret = call_or_ret;
        ftraces_max++;
    }
}

void print_ftrace()
{
    int func_stack = 0;
    for (int i = 0; i < ftraces_max; i++)
    {
        printf(FMT_WORD ":", ftraces[i].pc);
        if (ftraces[i].call_or_ret == 1)
            func_stack--;
        for (int j = 0; j < func_stack; j++)
            printf("|   ");
        if (ftraces[i].call_or_ret == 0)
        {
            printf("%s call %s " FMT_WORD, func_infos[ftraces[i].func_num].func_name,
                   func_infos[ftraces[i].nfunc_num].func_name, ftraces[i].npc);
            func_stack++;
        }
        else
        {
            printf("%s ret %s " FMT_WORD, func_infos[ftraces[i].func_num].func_name,
                   func_infos[ftraces[i].nfunc_num].func_name, ftraces[i].npc);
        }
        printf("\n");
    }
}
