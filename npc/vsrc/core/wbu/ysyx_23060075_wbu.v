`include "ysyx_23060075_isa.vh"

module ysyx_23060075_wbu (
    input wire clk,
    input wire rst,

    input  wire valid_1,
    output reg  ready_1,
    output reg  valid_2,
    input  wire ready_2,
    output wire wbu_start,

    input  wire [        `ysyx_23060075_IMM_WIDTH-1:0] imm,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] pc_imm,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] snpc,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] mem_r,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] csr_r,
    output wire [        `ysyx_23060075_ISA_WIDTH-1:0] srd,
    input  wire [`ysyx_23060075_SRD_MUX_SEL_WIDTH-1:0] srd_mux_sel
);

    always @(posedge clk) begin
        if (rst) ready_1 <= 1'b1;
        else if (ready_1 && valid_1) ready_1 <= 1'b0;
        else if (ready_2 && valid_2) ready_1 <= 1'b1;
    end
    always @(posedge clk) begin
        if (rst) valid_2 <= 1'b1;
        else if (ready_2 && valid_2) valid_2 <= 1'b0;
        else if (ready_1 && valid_1) valid_2 <= 1'b1;
    end
    ysyx_23060075_pluse pluse_wbu_start (
        .clk (clk),
        .rst (rst),
        .din (ready_1 && valid_1),
        .dout(wbu_start)
    );

    ysyx_23060075_wbu_core wbu_core_1 (
        .imm        (imm),
        .pc_imm     (pc_imm),
        .snpc       (snpc),
        .mem_r      (mem_r),
        .alu_result (alu_result),
        .csr_r      (csr_r),
        .srd        (srd),
        .srd_mux_sel(srd_mux_sel)
    );

endmodule
