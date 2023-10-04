`include "vsrc/config.v"

module pc
     (
         input wire clk,rst,wen,
         input wire [`ISA_WIDTH-1:0] din,
         output wire [`ISA_WIDTH-1:0] dout
     );
     
     Reg
        #(
            .WIDTH(`ISA_WIDTH),
            .RESET_VAL(`PC_BASE_ADDR)
        )
        reg_pc
        (
            .clk(clk),
            .rst(rst),
            .din(din),
            .dout(dout),
            .wen(wen)
        );
    
endmodule
