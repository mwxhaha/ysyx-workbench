module adder8
(
    input  wire [7:0]  in_x, in_y,
    output wire [7:0]  out_s,
    output wire out_c
);
    assign {out_c,out_s} = in_x+in_y;
    
endmodule
