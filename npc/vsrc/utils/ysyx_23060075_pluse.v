module ysyx_23060075_pluse (
    input  wire clk,
    input  wire rst,
    input  wire din,
    output wire dout
);

    reg din_reg;
    always @(posedge clk) begin
        if (rst) din_reg <= 1'b0;
        else din_reg <= din;
    end
    assign dout = din & ~din_reg;

endmodule
