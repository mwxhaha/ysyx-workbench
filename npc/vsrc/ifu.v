`include "config.v"

module ifu
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] mem_r,
        output wire [`ISA_WIDTH-1:0] inst
    );

    assign inst = mem_r ;

endmodule
