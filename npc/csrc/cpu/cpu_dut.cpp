#include <cpu/cpu_dut.hpp>

#include <cstddef>
#include <cstdbool>
#include <cstdint>
#include <cassert>
#include <dlfcn.h>

#include <sim/cpu_sim.hpp>
#include <cpu/cpu_reg.hpp>
#include <cpu/cpu_log.hpp>
#include <cpu/cpu_mem.hpp>

enum
{
    DIFFTEST_TO_DUT,
    DIFFTEST_TO_REF
};

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

static bool difftest_close = false;

void init_difftest(const char *ref_so_file, long img_size)
{
    if (!ref_so_file)
    {
        Log("No reference is given. Difftest will not work");
        difftest_close = true;
        return;
    }

    void *handle;
    handle = dlopen(ref_so_file, RTLD_LAZY);
    assert(handle);

    ref_difftest_memcpy = (void (*)(vaddr_t, void *, size_t, bool))dlsym(handle, "difftest_memcpy");
    assert(ref_difftest_memcpy);

    ref_difftest_regcpy = (void (*)(void *, bool))dlsym(handle, "difftest_regcpy");
    assert(ref_difftest_regcpy);

    ref_difftest_exec = (void (*)(uint64_t))dlsym(handle, "difftest_exec");
    assert(ref_difftest_exec);

    ref_difftest_raise_intr = (void (*)(uint64_t))dlsym(handle, "difftest_raise_intr");
    assert(ref_difftest_raise_intr);

    void (*ref_difftest_init)() = (void (*)())dlsym(handle, "difftest_init");
    assert(ref_difftest_init);

    Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
    Log("The result of every instruction will be compared with %s. "
        "This will help you a lot for debugging, but also significantly reduce the performance. "
        "If it is not necessary, you can turn it off in menuconfig.",
        ref_so_file);

    ref_difftest_init();
    ref_difftest_memcpy(MEM_BASE_ADDR, mem, img_size, DIFFTEST_TO_REF);

    CPU_state cpu;
    cpu.pc = MEM_BASE_ADDR;
    for (int i = 0; i < GPR_NUM; i++)
        cpu.gpr[i] = 0;
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}

static void checkregs(CPU_state *ref, vaddr_t pc)
{
    if (!isa_difftest_checkregs(ref, pc))
    {
        npc_state.state = NPC_ABORT;
        npc_state.halt_pc = pc;
        // isa_reg_display();
    }
}

void difftest_step(vaddr_t pc, vaddr_t npc)
{
    if (difftest_close)
        return;

    CPU_state ref_r;

    ref_difftest_exec(1);
    ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

    checkregs(&ref_r, pc);
}
