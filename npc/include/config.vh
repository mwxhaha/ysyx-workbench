`define ISA_WIDTH 32
`define REG_ADDR_WIDTH 4
`define OPCODE_WIDTH 7
`define IMM_WIDTH 32
`define SHAMT_WIDTH 5
`define FUNCT3_WIDTH 3
`define FUNCT7_WIDTH 7

`define BASE_ADDR `ISA_WIDTH'h80000000
`define PC_BASE_ADDR `BASE_ADDR

`define ALU_FUNCT_WIDTH 4
`define ALU_FUNCT_MAX 13
`define NO_FUNCT 0
`define ADD 1
`define SUB 2
`define ADD_U 3
`define SUB_U 4
`define NOT 5
`define AND 6
`define OR 7
`define XOR 8
`define EQ 9
`define NEQ 10
`define LESS_U 11
`define SHIFT_L_L 12
`define SHIFT_R_A 13
