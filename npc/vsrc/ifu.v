`include "vsrc/config.v"

module ifu
     (
         input wire clk,rst,
         input wire [`ISA_WIDTH-1:0] rdata,
         output wire [`ISA_WIDTH-1:0] inst
     );

    assign inst = rdata ;
    
endmodule
