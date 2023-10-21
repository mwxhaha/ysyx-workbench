`include "config.vh"
`include "inst.vh"

module idu (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] inst,
    output wire [`INST_NUM_WIDTH-1:0] inst_num,
    output wire [`INST_TYPE_WIDTH-1:0] inst_type,
    output wire [`REG_ADDR_WIDTH-1:0] rd,
    output wire [`REG_ADDR_WIDTH-1:0] rs1,
    output wire [`REG_ADDR_WIDTH-1:0] rs2,
    output wire [`IMM_WIDTH-1:0] imm,
    output wire [`SHAMT_WIDTH-1:0] shamt
);

    wire [`INST_NUM_WIDTH-1:0] inst_srli_addi_num;
    wire [`INST_NUM_WIDTH-1:0] inst_add_add_num;
    wire [`INST_NUM_WIDTH-1:0] inst_srl_add_num;
    wire [`INST_NUM_WIDTH-1:0] inst_ebreak_ebreak_num;
    idu_funct7 u_idu_funct7 (
        .clk                   (clk),
        .rst                   (rst),
        .inst                  (inst),
        .inst_srli_addi_num    (inst_srli_addi_num),
        .inst_add_add_num      (inst_add_add_num),
        .inst_srl_add_num      (inst_srl_add_num),
        .inst_ebreak_ebreak_num(inst_ebreak_ebreak_num)
    );

    wire [`INST_NUM_WIDTH-1:0] inst_beq_num;
    wire [`INST_NUM_WIDTH-1:0] inst_lb_num;
    wire [`INST_NUM_WIDTH-1:0] inst_sb_num;
    wire [`INST_NUM_WIDTH-1:0] inst_addi_num;
    wire [`INST_NUM_WIDTH-1:0] inst_add_num;
    wire [`INST_NUM_WIDTH-1:0] inst_ebreak_num;
    idu_funct3 idu_funct3_1 (
        .clk                   (clk),
        .rst                   (rst),
        .inst                  (inst),
        .inst_srli_addi_num    (inst_srli_addi_num),
        .inst_add_add_num      (inst_add_add_num),
        .inst_srl_add_num      (inst_srl_add_num),
        .inst_ebreak_ebreak_num(inst_ebreak_ebreak_num),
        .inst_beq_num          (inst_beq_num),
        .inst_lb_num           (inst_lb_num),
        .inst_sb_num           (inst_sb_num),
        .inst_addi_num         (inst_addi_num),
        .inst_add_num          (inst_add_num),
        .inst_ebreak_num       (inst_ebreak_num)
    );

    idu_opcode idu_opcode_1 (
        .clk            (clk),
        .rst            (rst),
        .inst           (inst),
        .inst_beq_num   (inst_beq_num),
        .inst_lb_num    (inst_lb_num),
        .inst_sb_num    (inst_sb_num),
        .inst_addi_num  (inst_addi_num),
        .inst_add_num   (inst_add_num),
        .inst_ebreak_num(inst_ebreak_num),
        .inst_num       (inst_num),
        .inst_type      (inst_type),
        .rd             (rd),
        .rs1            (rs1),
        .rs2            (rs2),
        .imm            (imm),
        .shamt          (shamt)
    );

endmodule
