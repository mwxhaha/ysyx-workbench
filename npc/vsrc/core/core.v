`include "config.vh"

module core (
    input  wire                       clk,
    input  wire                       rst,
    input  wire [     `ISA_WIDTH-1:0] mem_1_r,
    output wire [     `ISA_WIDTH-1:0] mem_1_addr,
    output wire                       mem_1_r_en,
    input  wire [     `ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ISA_WIDTH-1:0] mem_2_addr,
    output wire [`MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                       mem_2_r_en,
    output wire                       mem_2_w_en
);

    wire [`ISA_WIDTH-1:0] pc_imm;
    wire [`ISA_WIDTH-1:0] alu_result;
    wire                  is_branch;
    wire                  is_jal;
    wire                  is_jalr;
    wire [`ISA_WIDTH-1:0] pc;
    wire [`ISA_WIDTH-1:0] snpc;
    wire                  pc_en;
    wire                  mem_if_en;
    wire [`ISA_WIDTH-1:0] inst;

    ifu ifu_1 (
        .clk       (clk),
        .rst       (rst),
        .pc_imm    (pc_imm),
        .alu_result(alu_result),
        .is_branch (is_branch),
        .is_jal    (is_jal),
        .is_jalr   (is_jalr),
        .pc        (pc),
        .snpc      (snpc),
        .pc_en     (pc_en),
        .mem_1_r   (mem_1_r),
        .mem_1_addr(mem_1_addr),
        .mem_1_r_en(mem_1_r_en),
        .mem_if_en (mem_if_en),
        .inst      (inst)
    );

    wire [`INST_TYPE_WIDTH-1:0] inst_type;
    wire [      `IMM_WIDTH-1:0] imm;
    wire [   `OPCODE_WIDTH-1:0] opcode;
    wire [   `FUNCT3_WIDTH-1:0] funct3;
    wire [   `FUNCT7_WIDTH-1:0] funct7;
    wire [      `ISA_WIDTH-1:0] srd;
    wire [      `ISA_WIDTH-1:0] src1;
    wire [      `ISA_WIDTH-1:0] src2;
    wire                        gpr_w_en;

    idu idu_1 (
        .clk      (clk),
        .inst     (inst),
        .inst_type(inst_type),
        .imm      (imm),
        .opcode   (opcode),
        .funct3   (funct3),
        .funct7   (funct7),
        .srd      (srd),
        .src1     (src1),
        .src2     (src2),
        .gpr_w_en (gpr_w_en)
    );

    wire                        alu_b_is_imm;
    wire [`ALU_FUNCT_WIDTH-1:0] alu_funct;

    exu exu_1 (
        .imm         (imm),
        .src1        (src1),
        .src2        (src2),
        .alu_b_is_imm(alu_b_is_imm),
        .alu_funct   (alu_funct),
        .alu_result  (alu_result),
        .pc          (pc),
        .pc_imm      (pc_imm)
    );

    wire [     `ISA_WIDTH-1:0] mem_r;
    wire [`MEM_MASK_WIDTH-1:0] mem_mask;
    wire                       mem_r_en;
    wire                       mem_w_en;
    memu memu_1 (
        .alu_result(alu_result),
        .src2      (src2),
        .mem_r     (mem_r),
        .mem_mask  (mem_mask),
        .mem_r_en  (mem_r_en),
        .mem_w_en  (mem_w_en),
        .mem_2_r   (mem_2_r),
        .mem_2_w   (mem_2_w),
        .mem_2_addr(mem_2_addr),
        .mem_2_mask(mem_2_mask),
        .mem_2_r_en(mem_2_r_en),
        .mem_2_w_en(mem_2_w_en)
    );

    wire rd_is_mem;
    wire is_lui;
    wire is_auipc;

    wbu wbu_1 (
        .imm       (imm),
        .snpc      (snpc),
        .pc_imm    (pc_imm),
        .mem_r     (mem_r),
        .alu_result(alu_result),
        .srd       (srd),
        .rd_is_mem (rd_is_mem),
        .is_lui    (is_lui),
        .is_auipc  (is_auipc),
        .is_jal    (is_jal),
        .is_jalr   (is_jalr)
    );

    ctrl ctrl_1 (
        .inst_type   (inst_type),
        .opcode      (opcode),
        .funct3      (funct3),
        .funct7      (funct7),
        .pc_en       (pc_en),
        .is_branch   (is_branch),
        .is_jal      (is_jal),
        .is_jalr     (is_jalr),
        .mem_if_en   (mem_if_en),
        .gpr_w_en    (gpr_w_en),
        .alu_b_is_imm(alu_b_is_imm),
        .alu_funct   (alu_funct),
        .mem_r_en    (mem_r_en),
        .mem_mask    (mem_mask),
        .mem_w_en    (mem_w_en),
        .rd_is_mem   (rd_is_mem),
        .is_lui      (is_lui),
        .is_auipc    (is_auipc)
    );

endmodule
