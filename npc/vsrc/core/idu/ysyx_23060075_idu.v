`include "ysyx_23060075_isa.vh"

module ysyx_23060075_idu (
    input wire clk,
    input wire rst,

    input  wire valid_1,
    output reg  ready_1,
    output reg  valid_2,
    input  wire ready_2,

    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] inst,
    output wire [`ysyx_23060075_INST_TYPE_WIDTH-1:0] inst_type,

    output wire [`ysyx_23060075_IMM_WIDTH-1:0] imm,

    output wire [`ysyx_23060075_OPCODE_WIDTH-1:0] opcode,
    output wire [`ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    output wire [`ysyx_23060075_FUNCT7_WIDTH-1:0] funct7,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] srd,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] src1,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] src2,
    input  wire                                gpr_w_en,

    input wire is_csri,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] pc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mepc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mtvec,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] csr_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] intr_code,
    input  wire                                is_intr,
    input  wire                                csr_w_en
);

    wire idu_start;
    always @(posedge clk) begin
        if (rst) ready_1 <= 1'b1;
        else if (ready_1 && valid_1) ready_1 <= 1'b0;
        else if (ready_2 && valid_2) ready_1 <= 1'b1;
    end
    always @(posedge clk) begin
        if (rst) valid_2 <= 1'b0;
        else if (ready_2 && valid_2) valid_2 <= 1'b0;
        else if (ready_1 && valid_1) valid_2 <= 1'b1;
    end
    ysyx_23060075_pluse pluse_idu_start (
        .clk (clk),
        .rst (rst),
        .din (ready_1 && valid_1),
        .dout(idu_start)
    );

    ysyx_23060075_idu_core idu_core_1 (
        .clk      (clk),
        .rst      (rst),
        .inst     (inst),
        .inst_type(inst_type),
        .imm      (imm),
        .opcode   (opcode),
        .funct3   (funct3),
        .funct7   (funct7),
        .srd      (srd),
        .src1     (src1),
        .src2     (src2),
        .gpr_w_en (gpr_w_en),
        .is_csri  (is_csri),
        .pc       (pc),
        .mtvec    (mtvec),
        .mepc     (mepc),
        .csr_r    (csr_r),
        .intr_code(intr_code),
        .is_intr  (is_intr & idu_start),
        .csr_w_en (csr_w_en & idu_start)
    );

endmodule
