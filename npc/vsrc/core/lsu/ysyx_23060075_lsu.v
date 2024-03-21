`include "ysyx_23060075_isa.vh"

module ysyx_23060075_lsu (
    input wire clk,
    input wire rst,

    input  wire valid_1,
    output reg  ready_1,
    output reg  valid_2,
    input  wire ready_2,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] src2,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_r,
    input  wire [  `ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_mask,
    input  wire                                     mem_r_en,
    input  wire                                     mem_w_en,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                                     mem_2_r_en,
    output wire                                     mem_2_w_en
);

    wire lsu_start;
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
    ysyx_23060075_pluse pluse_lsu_start (
        .clk (clk),
        .rst (rst),
        .din (ready_1 && valid_1),
        .dout(lsu_start)
    );

    ysyx_23060075_lsu_core lsu_core_1 (
        .src2      (src2),
        .alu_result(alu_result),
        .mem_r     (mem_r),
        .funct3    (funct3),
        .mem_mask  (mem_mask),
        .mem_r_en  (mem_r_en & lsu_start),
        .mem_w_en  (mem_w_en & lsu_start),
        .mem_2_r   (mem_2_r),
        .mem_2_w   (mem_2_w),
        .mem_2_addr(mem_2_addr),
        .mem_2_mask(mem_2_mask),
        .mem_2_r_en(mem_2_r_en),
        .mem_2_w_en(mem_2_w_en)
    );

endmodule
