`include "ysyx_23060075_isa.vh"

module ysyx_23060075_exu (
    input wire clk,
    input wire rst,
    
    input  wire valid_1,
    output reg  ready_1,
    output reg  valid_2,
    input  wire ready_2,

    input  wire [      `ysyx_23060075_IMM_WIDTH-1:0] imm,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] src1,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] src2,
    output wire [      `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input  wire                                      alu_b_is_imm,
    input  wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] pc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pc_imm
);

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

    ysyx_23060075_exu_core exu_core_1 (
        .imm         (imm),
        .src1        (src1),
        .src2        (src2),
        .alu_result  (alu_result),
        .alu_b_is_imm(alu_b_is_imm),
        .alu_funct   (alu_funct),
        .pc          (pc),
        .pc_imm      (pc_imm)
    );

endmodule
