module ysyx_23060075_register #(
    parameter WIDTH = 1,
    parameter RESET_VAL = 0
) (
    input  wire             clk,
    input  wire             rst,
    input  wire [WIDTH-1:0] din,
    output reg  [WIDTH-1:0] dout,
    input  wire             wen
);

    always @(posedge clk) begin
        if (rst) dout <= RESET_VAL;
        else if (wen) dout <= din;
    end

endmodule
