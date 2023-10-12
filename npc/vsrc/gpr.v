`include "config.v"

module gpr
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] gpr_w,
        input wire [`REG_ADDR_WIDTH -1:0] gpr_w_addr,
        output wire [`ISA_WIDTH-1:0] gpr_r_1,
        input wire [`REG_ADDR_WIDTH -1:0] gpr_r_1_addr,
        output wire [`ISA_WIDTH-1:0] gpr_r_2,
        input wire [`REG_ADDR_WIDTH -1:0] gpr_r_2_addr,
        input wire gpr_w_en
    );

    wire [`ISA_WIDTH-1:0] rdata_1;
    wire [`ISA_WIDTH-1:0] rdata_2;
    RegisterFile
        #(
            .ADDR_WIDTH(`REG_ADDR_WIDTH ),
            .DATA_WIDTH(`ISA_WIDTH)
        )
        registerfile_gpr
        (
            .clk(clk),
            .rst(rst),
            .wdata(gpr_w),
            .waddr(gpr_w_addr),
            .rdata_1(rdata_1),
            .raddr_1(gpr_r_1_addr),
            .rdata_2(rdata_2),
            .raddr_2(gpr_r_2_addr),
            .wen(gpr_w_en)
        );
    
    wire gpr_r_1_addr_is_not_zero=|(gpr_r_1_addr^`REG_ADDR_WIDTH'b0);
    assign gpr_r_1={`ISA_WIDTH{gpr_r_1_addr_is_not_zero}}&rdata_1;
    wire gpr_r_2_addr_is_not_zero=|(gpr_r_2_addr^`REG_ADDR_WIDTH'b0);
    assign gpr_r_2={`ISA_WIDTH{gpr_r_2_addr_is_not_zero}}&rdata_2;

endmodule
