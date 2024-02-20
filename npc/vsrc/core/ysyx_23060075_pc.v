`include "config.vh"

module ysyx_23060075_pc (
    input  wire                                clk,
    input  wire                                rst,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] pc_in,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pc_out,
    input  wire                                pc_en
);

    ysyx_23060075_register #(
        .WIDTH    (`ysyx_23060075_ISA_WIDTH),
        .RESET_VAL(`ysyx_23060075_PC_BASE_ADDR)
    ) register_pc (
        .clk (clk),
        .rst (rst),
        .din (pc_in),
        .dout(pc_out),
        .wen (pc_en)
    );

endmodule
