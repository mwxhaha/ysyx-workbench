`define INST_SRLI_ADDI_NUM_MAX 1
`define srai 30

`define INST_ADD_ADD_NUM_MAX 2
`define add 31
`define sub 32

`define INST_EBREAK_EBREAK_NUM_MAX 1
`define ebreak 42



`define INST_BEQ_NUM_MAX 2
`define beq 5
`define bne 6

`define INST_LB_NUM_MAX 2
`define lw 13
`define lbu 14

`define INST_SB_NUM_MAX 2
`define sh 19
`define sw 20

`define INST_ADDI_NUM_MAX 4
`define INST_ADDI_NUM_IDU_MAX `INST_ADDI_NUM_MAX-`INST_SRLI_ADDI_NUM_MAX+1
`define addi 21
`define sltiu 23
`define andi 26

`define INST_ADD_NUM_MAX 5
`define INST_ADD_NUM_IDU_MAX `INST_ADD_NUM_MAX-`INST_ADD_ADD_NUM_MAX+1
`define sltu 35
`define ixor 36
`define ior 37

`define INST_EBREAK_NUM_MAX 1
`define INST_EBREAK_NUM_IDU_MAX `INST_EBREAK_NUM_MAX-`INST_EBREAK_EBREAK_NUM_MAX+1



`define INST_NUM_WIDTH 8
`define INST_NUM_MAX 19
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
