`define INST_SRLI_ADDI_NUM_MAX 2
`define srli 26
`define srai 27

`define INST_ADD_ADD_NUM_MAX 2
`define add 28
`define sub 29

`define INST_EBREAK_EBREAK_NUM_MAX 1
`define ebreak 42



`define INST_BEQ_NUM_MAX 6
`define beq 5
`define bne 6
`define blt 7
`define bge 8
`define bltu 9
`define bgeu 10

`define INST_LB_NUM_MAX 2
`define lw 13
`define lbu 14

`define INST_SB_NUM_MAX 3
`define sb 16
`define sh 17
`define sw 18

`define INST_ADDI_NUM_MAX 7
`define INST_ADDI_NUM_IDU_MAX `INST_ADDI_NUM_MAX-`INST_SRLI_ADDI_NUM_MAX+1
`define addi 19
`define sltiu 21
`define xori 22
`define andi 24
`define slli 25

`define INST_ADD_NUM_MAX 8
`define INST_ADD_NUM_IDU_MAX `INST_ADD_NUM_MAX-`INST_ADD_ADD_NUM_MAX+1
`define sll 30
`define slt 31
`define sltu 32
`define ixor 33
`define ior 36
`define iand 37

`define INST_EBREAK_NUM_MAX 1
`define INST_EBREAK_NUM_IDU_MAX `INST_EBREAK_NUM_MAX-`INST_EBREAK_EBREAK_NUM_MAX+1



`define INST_NUM_WIDTH 8
`define INST_NUM_MAX 31
`define INST_NUM_IDU_MAX `INST_NUM_MAX-`INST_BEQ_NUM_MAX+1-`INST_LB_NUM_MAX+1-`INST_SB_NUM_MAX+1-`INST_ADDI_NUM_MAX+1-`INST_ADD_NUM_MAX+1
`define inv 0
`define lui 1
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
