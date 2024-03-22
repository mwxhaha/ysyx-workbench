`include "ysyx_23060075_isa.vh"

module ysyx_23060075_core (
    input  wire                                     clk,
    input  wire                                     rst,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    output wire                                     mem_1_r_en,
    input  wire                                     mem_1_finish,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                                     mem_2_r_en,
    output wire                                     mem_2_w_en
);

    wire                                         valid_wbu_ifu;
    wire                                         ready_wbu_ifu;
    wire                                         valid_ifu_idu;
    wire                                         ready_ifu_idu;
    wire                                         wbu_start;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] pc_imm;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] alu_result;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] mtvec;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] mepc;
    wire [`ysyx_23060075_DNPC_MUX_SEL_WIDTH-1:0] dnpc_mux_sel;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] pc;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] snpc;
    wire                                         pc_en;
    wire [         `ysyx_23060075_ISA_WIDTH-1:0] inst;
    wire                                         mem_if_en;
    ysyx_23060075_ifu ifu_1 (
        .clk         (clk),
        .rst         (rst),
        .valid_1     (valid_wbu_ifu),
        .ready_1     (ready_wbu_ifu),
        .valid_2     (valid_ifu_idu),
        .ready_2     (ready_ifu_idu),
        .pc_imm      (pc_imm),
        .alu_result  (alu_result),
        .mtvec       (mtvec),
        .mepc        (mepc),
        .dnpc_mux_sel(dnpc_mux_sel),
        .pc          (pc),
        .snpc        (snpc),
        .pc_en       (pc_en & wbu_start),
        .inst        (inst),
        .mem_if_en   (mem_if_en),
        .mem_1_r     (mem_1_r),
        .mem_1_addr  (mem_1_addr),
        .mem_1_r_en  (mem_1_r_en),
        .mem_1_finish(mem_1_finish)
    );

    wire                                      valid_idu_exu;
    wire                                      ready_idu_exu;
    wire [`ysyx_23060075_INST_TYPE_WIDTH-1:0] inst_type;
    wire [      `ysyx_23060075_IMM_WIDTH-1:0] imm;
    wire [   `ysyx_23060075_OPCODE_WIDTH-1:0] opcode;
    wire [   `ysyx_23060075_FUNCT3_WIDTH-1:0] funct3;
    wire [   `ysyx_23060075_FUNCT7_WIDTH-1:0] funct7;
    wire [      `ysyx_23060075_ISA_WIDTH-1:0] srd;
    wire [      `ysyx_23060075_ISA_WIDTH-1:0] src1;
    wire [      `ysyx_23060075_ISA_WIDTH-1:0] src2;
    wire                                      gpr_w_en;
    wire                                      is_csri;
    wire [      `ysyx_23060075_ISA_WIDTH-1:0] csr_r;
    wire [      `ysyx_23060075_ISA_WIDTH-1:0] intr_code;
    wire                                      is_intr;
    wire                                      csr_w_en;
    ysyx_23060075_idu idu_1 (
        .clk      (clk),
        .rst      (rst),
        .valid_1  (valid_ifu_idu),
        .ready_1  (ready_ifu_idu),
        .valid_2  (valid_idu_exu),
        .ready_2  (ready_idu_exu),
        .inst     (inst),
        .inst_type(inst_type),
        .imm      (imm),
        .opcode   (opcode),
        .funct3   (funct3),
        .funct7   (funct7),
        .srd      (srd),
        .src1     (src1),
        .src2     (src2),
        .gpr_w_en (gpr_w_en & wbu_start),
        .is_csri  (is_csri),
        .pc       (pc),
        .mtvec    (mtvec),
        .mepc     (mepc),
        .csr_r    (csr_r),
        .intr_code(intr_code),
        .is_intr  (is_intr),
        .csr_w_en (csr_w_en)
    );

    wire                                      valid_exu_lsu;
    wire                                      ready_exu_lsu;
    wire                                      alu_b_is_imm;
    wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct;
    ysyx_23060075_exu exu_1 (
        .clk         (clk),
        .rst         (rst),
        .valid_1     (valid_idu_exu),
        .ready_1     (ready_idu_exu),
        .valid_2     (valid_exu_lsu),
        .ready_2     (ready_exu_lsu),
        .imm         (imm),
        .src1        (src1),
        .src2        (src2),
        .alu_result  (alu_result),
        .alu_b_is_imm(alu_b_is_imm),
        .alu_funct   (alu_funct),
        .pc          (pc),
        .pc_imm      (pc_imm)
    );

    wire                                     valid_lsu_wbu;
    wire                                     ready_lsu_wbu;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_r;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_mask;
    wire                                     mem_r_en;
    wire                                     mem_w_en;
    ysyx_23060075_lsu lsu_1 (
        .clk       (clk),
        .rst       (rst),
        .valid_1   (valid_exu_lsu),
        .ready_1   (ready_exu_lsu),
        .valid_2   (valid_lsu_wbu),
        .ready_2   (ready_lsu_wbu),
        .src2      (src2),
        .alu_result(alu_result),
        .mem_r     (mem_r),
        .funct3    (funct3),
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

    wire [`ysyx_23060075_SRD_MUX_SEL_WIDTH-1:0] srd_mux_sel;
    ysyx_23060075_wbu wbu_1 (
        .clk        (clk),
        .rst        (rst),
        .valid_1    (valid_lsu_wbu),
        .ready_1    (ready_lsu_wbu),
        .valid_2    (valid_wbu_ifu),
        .ready_2    (ready_wbu_ifu),
        .wbu_start  (wbu_start),
        .imm        (imm),
        .pc_imm     (pc_imm),
        .snpc       (snpc),
        .mem_r      (mem_r),
        .alu_result (alu_result),
        .csr_r      (csr_r),
        .srd        (srd),
        .srd_mux_sel(srd_mux_sel)
    );

    ysyx_23060075_ctrl ctrl_1 (
        .inst        (inst),
        .dnpc_mux_sel(dnpc_mux_sel),
        .pc_en       (pc_en),
        .mem_if_en   (mem_if_en),
        .inst_type   (inst_type),
        .opcode      (opcode),
        .funct3      (funct3),
        .funct7      (funct7),
        .gpr_w_en    (gpr_w_en),
        .is_csri     (is_csri),
        .csr_w_en    (csr_w_en),
        .intr_code   (intr_code),
        .is_intr     (is_intr),
        .alu_b_is_imm(alu_b_is_imm),
        .alu_funct   (alu_funct),
        .mem_mask    (mem_mask),
        .mem_r_en    (mem_r_en),
        .mem_w_en    (mem_w_en),
        .srd_mux_sel (srd_mux_sel)
    );

endmodule
