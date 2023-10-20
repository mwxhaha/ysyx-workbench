`include "config.vh"

module gpr (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] gpr_w,
    input wire [`REG_ADDR_WIDTH -1:0] gpr_w_addr,
    output wire [`ISA_WIDTH-1:0] gpr_r_1,
    input wire [`REG_ADDR_WIDTH -1:0] gpr_r_1_addr,
    output wire [`ISA_WIDTH-1:0] gpr_r_2,
    input wire [`REG_ADDR_WIDTH -1:0] gpr_r_2_addr,
    input wire gpr_w_en
);

    wire gpr_w_addr_is_not_zero = |(gpr_w_addr ^ `REG_ADDR_WIDTH'b0);
    wire [`ISA_WIDTH-1:0] wdata = gpr_w & {`ISA_WIDTH{gpr_w_addr_is_not_zero}};

    RegisterFile #(
        .ADDR_WIDTH(`REG_ADDR_WIDTH),
        .DATA_WIDTH(`ISA_WIDTH)
    ) registerfile_gpr (
        .clk(clk),
        .rst(rst),
        .wdata(wdata),
        .waddr(gpr_w_addr),
        .rdata_1(gpr_r_1),
        .raddr_1(gpr_r_1_addr),
        .rdata_2(gpr_r_2),
        .raddr_2(gpr_r_2_addr),
        .wen(gpr_w_en)
    );

endmodule
