#include <cpu/cpu_dut.hpp>

#include <cstddef>
#include <cassert>
#include <cstdio>
#include <cstdbool>
#include <dlfcn.h>

#include <sim/cpu_sim.hpp>
#include <util/sim_tool.hpp>
#include <cpu/cpu_reg.hpp>
#include <Vtop.h>
#include <Vtop___024root.h>

#define DIFFTEST_TO_DUT 0
#define DIFFTEST_TO_REF 1

void (*ref_difftest_memcpy)(vaddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

void init_difftest(const char *ref_so_file, long img_size)
{
    assert(ref_so_file != NULL);

    void *handle;
    handle = dlopen(ref_so_file, RTLD_LAZY);
    assert(handle);

    ref_difftest_memcpy = (void (*)(vaddr_t addr, void *buf, size_t n, bool direction))dlsym(handle, "difftest_memcpy");
    assert(ref_difftest_memcpy);

    ref_difftest_regcpy = (void (*)(void *dut, bool direction))dlsym(handle, "difftest_regcpy");
    assert(ref_difftest_regcpy);

    ref_difftest_exec = (void (*)(uint64_t n))dlsym(handle, "difftest_exec");
    assert(ref_difftest_exec);

    ref_difftest_raise_intr = (void (*)(uint64_t NO))dlsym(handle, "difftest_raise_intr");
    assert(ref_difftest_raise_intr);

    void (*ref_difftest_init)() = (void (*)())dlsym(handle, "difftest_init");
    assert(ref_difftest_init);

    printf("Differential testing: ON\n");
    printf("The result of every instruction will be compared with %s. "
        "This will help you a lot for debugging, but also significantly reduce the performance. "
        "If it is not necessary, you can turn it off in makefile.\n",
        ref_so_file);

    ref_difftest_init();
    ref_difftest_memcpy(MEM_BASE_ADDR, mem, img_size, DIFFTEST_TO_REF);
            
    CPU_state cpu;
    cpu.pc = MEM_BASE_ADDR;
    for (int i = 0; i < GPR_NUM; i++)
        cpu.gpr[i] = 0;
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}

static void checkregs(CPU_state *ref, vaddr_t pc) {
  if (!isa_difftest_checkregs(ref, pc)) {
    npc_state.state = npc_absort;
    npc_state.pc = pc;
  }
}

void difftest_step(vaddr_t pc, vaddr_t npc) {
  CPU_state ref_r;

  ref_difftest_exec(1);
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

  checkregs(&ref_r, pc);
}
