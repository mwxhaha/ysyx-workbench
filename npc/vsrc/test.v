module test
    (
        input wire clk,rst,in,
        output reg q
    );

    always @(posedge clk)
    begin
        if(rst)
        begin
            q <= 1'b0;
        end
        else
        begin
            q <= in;
        end
    end

endmodule
