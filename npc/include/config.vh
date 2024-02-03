`define ISA_WIDTH 32
`define REG_ADDR_WIDTH 4
`define OPCODE_WIDTH 7
`define IMM_WIDTH `ISA_WIDTH
`define SHAMT_WIDTH 5
`define FUNCT3_WIDTH 3
`define FUNCT7_WIDTH 7

`define BASE_ADDR `ISA_WIDTH'h80000000
`define PC_BASE_ADDR `BASE_ADDR
`define MEM_MASK_WIDTH 4

`define ALU_FUNCT_WIDTH 4
`define ALU_FUNCT_MAX 14
`define NO_FUNCT 15
`define ADD 0
`define SUB 8
`define LT 2
`define LTU 3
`define SLL 1
`define SRL 5
`define SRA 13
`define XOR 4
`define OR 6
`define AND 7
`define EQ 9
`define NE 10
`define GE 11
`define GEU 12

`define INST_TYPE_WIDTH 3
`define INST_TYPE_MAX 6
`define N 0
`define R 1
`define I 2
`define S 3
`define B 4
`define U 5
`define J 6

`define OPCODE_NUMBER_MAX 10
