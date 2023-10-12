`define ISA_WIDTH 32
`define REG_ADDR_WIDTH 5
`define OPCODE_WIDTH 7
`define IMM_WIDTH 32
`define FUNCT3_WIDTH 3
`define FUNCT7_WIDTH 7

`define BASE_ADDR `ISA_WIDTH'h80000000
`define PC_BASE_ADDR `BASE_ADDR

`define INST_ADD_ADD_NUM_MAX 2
`define add 31
`define sub 32

`define INST_EBREAK_EBREAK_NUM_MAX 1
`define ebreak 42

`define INST_BEQ_NUM_MAX 2
`define beq 5
`define bne 6

`define INST_LB_NUM_MAX 1
`define lw 13

`define INST_SB_NUM_MAX 1
`define sw 20

`define INST_ADDI_NUM_MAX 2
`define addi 21
`define sltiu 23

`define INST_ADD_NUM_MAX 2
`define INST_ADD_NUM_IDU_MAX `INST_ADD_NUM_MAX-`INST_ADD_ADD_NUM_MAX+1

`define INST_EBREAK_NUM_MAX 1
`define INST_EBREAK_NUM_IDU_MAX `INST_EBREAK_NUM_MAX-`INST_EBREAK_EBREAK_NUM_MAX+1

`define INST_NUM_WIDTH 8
`define INST_NUM_MAX 12
`define INST_NUM_IDU_MAX `INST_NUM_MAX-`INST_BEQ_NUM_MAX+1-`INST_LB_NUM_MAX+1-`INST_SB_NUM_MAX+1-`INST_ADDI_NUM_MAX+1-`INST_ADD_NUM_MAX+1
`define inv 0
`define auipc 2
`define jal 3
`define jalr 4

`define INST_TYPE_WIDTH 3
`define INST_TYPE_MAX 6
`define N 0
`define R 1
`define I 2
`define S 3
`define B 4
`define U 5
`define J 6

`define ALU_FUNC_WIDTH 4
`define ALU_FUNC_MAX 11
`define NO_FUNC 0
`define ADD 1
`define SUB 2
`define ADD_U 3
`define SUB_U 4
`define NOT 5
`define AND 6
`define OR 7
`define XOR 8
`define EQ 9
`define NE 10
`define LESS_U 11
