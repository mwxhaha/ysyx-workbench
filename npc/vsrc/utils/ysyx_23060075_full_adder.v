module ysyx_23060075_full_adder (
    input  wire a,
    input  wire b,
    input  wire cin,
    output wire s,
    output wire cout
);

    wire s_1, c_1, c_2;
    ysyx_23060075_half_adder half_adder_s_1 (
        .a(a),
        .b(b),
        .s(s_1),
        .c(c_1)
    );
    ysyx_23060075_half_adder half_adder_s (
        .a(cin),
        .b(s_1),
        .s(s),
        .c(c_2)
    );
    assign cout = c_1 | c_2;

endmodule
