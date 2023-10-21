#ifndef CPU_DUT_HPP
#define CPU_DUT_HPP

#include <sim/cpu_sim.hpp>

void init_difftest(const char *ref_so_file, long img_size);
void difftest_step(vaddr_t pc, vaddr_t npc);

#endif
