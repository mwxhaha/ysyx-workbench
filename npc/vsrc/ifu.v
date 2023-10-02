module ifu
    #(
         parameter ISA_WIDTH = 32
     )
     (
         input wire clk,rst,
         input wire [ISA_WIDTH-1:0] pc_memory_read,
         output wire [ISA_WIDTH-1:0] pc_out,instruction
     );
    wire [ISA_WIDTH-1:0] pc_in;

    Reg
        #(
            .WIDTH(ISA_WIDTH),
            .RESET_VAL('h80000000)
        )
        reg_pc
        (
            .clk(clk),
            .rst(rst),
            .din(pc_in),
            .dout(pc_out),
            .wen(1'b1)
        );
    assign instruction = pc_memory_read ;
    assign pc_in = pc_out + 'd4;
endmodule
