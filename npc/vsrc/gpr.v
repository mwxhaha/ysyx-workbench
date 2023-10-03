module gpr
    #(
         parameter ISA_WIDTH = 32,
         parameter REGISTER_NUM_WIDTH = 5
     )
     (
         input wire clk,rst,
         input wire [ISA_WIDTH-1:0] wdata,
         input wire [REGISTER_NUM_WIDTH-1:0] waddr,
         output wire [ISA_WIDTH-1:0] rdata_1,
         input wire [REGISTER_NUM_WIDTH-1:0] raddr_1,
         output wire [ISA_WIDTH-1:0] rdata_2,
         input wire [REGISTER_NUM_WIDTH-1:0] raddr_2,
         input wire wen
     );

    RegisterFile
        #(
            .ADDR_WIDTH(REGISTER_NUM_WIDTH),
            .DATA_WIDTH(ISA_WIDTH)
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
