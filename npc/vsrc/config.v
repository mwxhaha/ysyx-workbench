`define ISA_WIDTH 32
`define REG_ADDR_WIDTH 5
`define OPCODE_WIDTH 7
`define IMM_WIDTH 32

`define BASE_ADDR `ISA_WIDTH'h80000000
`define PC_BASE_ADDR `BASE_ADDR

`define INST_NUM_WIDTH 8
`define INST_NUM_MAX 7
`define inv 0
`define auipc 2
`define jal 3
`define beq 5
`define sw 20
`define addi 21
`define add 31
`define ebreak 42

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
`define ALU_FUNC_MAX 8
`define NO_FUNC 0
`define ADD_S 0
`define SUB_S 1
`define ADD_U 2
`define SUB_U 3
`define NOT 4
`define AND 5
`define OR 6
`define XOR 7
