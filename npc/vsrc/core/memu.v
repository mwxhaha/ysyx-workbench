`include "config.vh"

module memu (
    input  wire [     `ISA_WIDTH-1:0] alu_result,
    input  wire [     `ISA_WIDTH-1:0] src2,
    output wire [     `ISA_WIDTH-1:0] mem_r,
    input  wire [`MEM_MASK_WIDTH-1:0] mem_mask,
    input  wire                       mem_r_en,
    input  wire                       mem_w_en,
    input  wire [     `ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ISA_WIDTH-1:0] mem_2_addr,
    output wire [`MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                       mem_2_r_en,
    output wire                       mem_2_w_en
);

    assign mem_r = mem_2_r;
    assign mem_2_w = src2;
    assign mem_2_addr = alu_result;
    assign mem_2_mask = mem_mask;
    assign mem_2_r_en = mem_r_en;
    assign mem_2_w_en = mem_w_en;

endmodule
