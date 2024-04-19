`include "ysyx_23060075_isa.vh"

module ysyx_23060075_ifu (
    input wire clk,
    input wire rst,

    input  wire valid_1,
    output reg  ready_1,
    output reg  valid_2,
    input  wire ready_2,

    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] pc_imm,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] mtvec,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] mepc,
    input wire [`ysyx_23060075_DNPC_MUX_SEL_WIDTH-1:0] dnpc_mux_sel,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] snpc,
    input  wire                                pc_en,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] inst,
    input  wire                                mem_if_en,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    output wire                                mem_1_r_en,
    input  wire                                mem_1_finish
);

    wire ifu_start;
    always @(posedge clk, posedge rst) begin
        if (rst) ready_1 <= 1'b1;
        else if (ready_1 && valid_1) ready_1 <= 1'b0;
        else if (ready_2 && valid_2) ready_1 <= 1'b1;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) valid_2 <= 1'b0;
        else if (ready_2 && valid_2) valid_2 <= 1'b0;
        else if (mem_1_finish) valid_2 <= 1'b1;
    end
    ysyx_23060075_pluse pluse_ifu_start (
        .clk (clk),
        .rst (rst),
        .din (ready_1 && valid_1),
        .dout(ifu_start)
    );

    ysyx_23060075_ifu_core ifu_core_1 (
        .clk         (clk),
        .rst         (rst),
        .pc_imm      (pc_imm),
        .alu_result  (alu_result),
        .mtvec       (mtvec),
        .mepc        (mepc),
        .dnpc_mux_sel(dnpc_mux_sel),
        .pc          (pc),
        .snpc        (snpc),
        .pc_en       (pc_en),
        .inst        (inst),
        .mem_if_en   (mem_if_en & ifu_start),
        .mem_1_r     (mem_1_r),
        .mem_1_addr  (mem_1_addr),
        .mem_1_r_en  (mem_1_r_en)
    );

endmodule
