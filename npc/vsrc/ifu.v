`include "vsrc/config.v"

module ifu
     (
         input wire clk,rst,
         input wire [`ISA_WIDTH-1:0] pc_mem_read,
         output wire [`ISA_WIDTH-1:0] pc_out,inst
     );

    wire [`ISA_WIDTH-1:0] pc_in;

    Reg
        #(
            .WIDTH(`ISA_WIDTH),
            .RESET_VAL(`PC_BASE_ADDR)
        )
        reg_pc
        (
            .clk(clk),
            .rst(rst),
            .din(pc_in),
            .dout(pc_out),
            .wen(1'b1)
        );

    assign inst = pc_mem_read ;
    assign pc_in = pc_out + `ISA_WIDTH'd4;
    
endmodule
