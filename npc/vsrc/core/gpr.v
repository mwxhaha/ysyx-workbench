`include "config.vh"

module gpr (
    input  wire                        clk,
    input  wire [      `ISA_WIDTH-1:0] gpr_w,
    output wire [      `ISA_WIDTH-1:0] gpr_1_r,
    output wire [      `ISA_WIDTH-1:0] gpr_2_r,
    input  wire [`REG_ADDR_WIDTH -1:0] gpr_w_addr,
    input  wire [`REG_ADDR_WIDTH -1:0] gpr_1_addr,
    input  wire [`REG_ADDR_WIDTH -1:0] gpr_2_addr,
    input  wire                        gpr_w_en
);

    wire                  gpr_w_addr_is_not_zero = |(gpr_w_addr ^ `REG_ADDR_WIDTH'b0);
    wire                  gpr_1_addr_is_not_zero = |(gpr_1_addr ^ `REG_ADDR_WIDTH'b0);
    wire                  gpr_2_addr_is_not_zero = |(gpr_2_addr ^ `REG_ADDR_WIDTH'b0);
    wire [`ISA_WIDTH-1:0] wdata = gpr_w & {`ISA_WIDTH{gpr_w_addr_is_not_zero}};
    wire [`ISA_WIDTH-1:0] rdata_1;
    wire [`ISA_WIDTH-1:0] rdata_2;
    assign gpr_1_r = rdata_1 & {`ISA_WIDTH{gpr_1_addr_is_not_zero}};
    assign gpr_2_r = rdata_2 & {`ISA_WIDTH{gpr_2_addr_is_not_zero}};

    reg_file #(
        .ADDR_WIDTH(`REG_ADDR_WIDTH),
        .DATA_WIDTH(`ISA_WIDTH)
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
