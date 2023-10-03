`include "vsrc/config.v"
module gpr
     (
         input wire clk,rst,
         input wire [`ISA_WIDTH-1:0] wdata,
         input wire [`REG_ADDR_WIDTH -1:0] waddr,
         output wire [`ISA_WIDTH-1:0] rdata_1,
         input wire [`REG_ADDR_WIDTH -1:0] raddr_1,
         output wire [`ISA_WIDTH-1:0] rdata_2,
         input wire [`REG_ADDR_WIDTH -1:0] raddr_2,
         input wire wen
     );

    RegisterFile
        #(
            .ADDR_WIDTH(`REG_ADDR_WIDTH ),
            .DATA_WIDTH(`ISA_WIDTH)
        )
        registerfile_gpr
        (
            .clk(clk),
            .rst(rst),
            .wdata(wdata),
            .waddr(waddr),
            .rdata_1(rdata_1),
            .raddr_1(raddr_1),
            .rdata_2(rdata_2),
            .raddr_2(raddr_2),
            .wen(wen)
        );

endmodule
