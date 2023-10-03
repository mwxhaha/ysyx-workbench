`define ISA_WIDTH 32
`define INST_NUM_WIDTH 8
`define INST_TYPE_WIDTH 3
`define REG_ADDR_WIDTH 5
`define OPCODE_WIDTH 7
`define IMM_WIDTH 32

`define PC_BASE_ADDR `ISA_WIDTH'h80000000

`define INST_NUM_MAX 7
`define inv 0
`define auipc 2
`define jal 3
`define beq 5
`define sw 20
`define addi 21
`define add 31
`define ebreak 42

`define INST_TYPE_MAX 6
`define N 0
`define R 1
`define I 2
`define S 3
`define B 4
`define U 5
`define J 6
