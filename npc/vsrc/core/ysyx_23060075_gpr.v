`include "config.vh"

module ysyx_23060075_gpr (
    input wire clk,
    // verilator lint_off UNUSEDSIGNAL
    input wire rst,
    // verilator lint_on UNUSEDSIGNAL

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] gpr_w,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] gpr_1_r,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] gpr_2_r,
    input  wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] gpr_w_addr,
    input  wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] gpr_1_addr,
    input  wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] gpr_2_addr,
    input  wire                                     gpr_w_en
);

    wire gpr_w_addr_is_not_zero = |(gpr_w_addr ^ `ysyx_23060075_REG_ADDR_WIDTH'b0);
    wire gpr_1_addr_is_not_zero = |(gpr_1_addr ^ `ysyx_23060075_REG_ADDR_WIDTH'b0);
    wire gpr_2_addr_is_not_zero = |(gpr_2_addr ^ `ysyx_23060075_REG_ADDR_WIDTH'b0);
    wire [`ysyx_23060075_ISA_WIDTH-1:0] wdata = gpr_w & {`ysyx_23060075_ISA_WIDTH{gpr_w_addr_is_not_zero}};
    wire [`ysyx_23060075_ISA_WIDTH-1:0] rdata_1;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] rdata_2;
    assign gpr_1_r = rdata_1 & {`ysyx_23060075_ISA_WIDTH{gpr_1_addr_is_not_zero}};
    assign gpr_2_r = rdata_2 & {`ysyx_23060075_ISA_WIDTH{gpr_2_addr_is_not_zero}};

    ysyx_23060075_reg_file #(
        .ADDR_WIDTH(`ysyx_23060075_REG_ADDR_WIDTH),
        .DATA_WIDTH(`ysyx_23060075_ISA_WIDTH)
    ) reg_file_gpr (
        .clk    (clk),
        .wdata  (wdata),
        .waddr  (gpr_w_addr),
        .rdata_1(rdata_1),
        .raddr_1(gpr_1_addr),
        .rdata_2(rdata_2),
        .raddr_2(gpr_2_addr),
        .wen    (gpr_w_en)
    );

endmodule
