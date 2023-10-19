#ifndef CPU_DISASM_HPP
#define CPU_DISASM_HPP

#include<cstdint>

void init_disasm(const char *triple);
void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

#endif