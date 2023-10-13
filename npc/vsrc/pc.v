`include "config.vh"

module pc (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] pc_in,
    output wire [`ISA_WIDTH-1:0] pc_out,
    input wire pc_w_en
);

    Reg #(
        .WIDTH(`ISA_WIDTH),
        .RESET_VAL(`PC_BASE_ADDR)
    ) reg_pc (
        .clk (clk),
        .rst (rst),
        .din (pc_in),
        .dout(pc_out),
        .wen (pc_w_en)
    );

endmodule
