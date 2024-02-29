module ysyx_23060075_adder_alu #(
    parameter data_len = 4
) (
    input  wire [data_len-1:0] a,
    input  wire [data_len-1:0] b,
    input  wire                is_sub,
    output wire [data_len-1:0] result,
    output wire                carry,
    output wire                overflow
);

    wire [data_len-1:0] b_1s_complement = b ^ {data_len{is_sub}};
    wire cout;
    ysyx_23060075_adder #(
        .data_len(data_len)
    ) adder_1 (
        .a   (a),
        .b   (b_1s_complement),
        .cin (is_sub),
        .s   (result),
        .cout(cout)
    );

    assign carry = cout ^ is_sub;
    assign overflow = (a[data_len-1] == b_1s_complement[data_len-1]) && (result[data_len-1] != a[data_len-1]);

endmodule
