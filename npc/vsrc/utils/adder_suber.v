module adder_suber #(
    parameter data_len = 4
) (
    input wire [data_len-1:0] a,
    input wire [data_len-1:0] b,
    input wire add_or_sub,
    output wire [data_len-1:0] result,
    output wire carry,
    output wire zero,
    output wire overflow
);

    wire [data_len-1:0] b_2_s_complement;
    wire cout;

    assign b_2_s_complement = b ^ {data_len{add_or_sub}};

    adder #(
        .data_len(data_len)
    ) adder_i (
        .a   (a),
        .b   (b_2_s_complement),
        .cin (add_or_sub),
        .s   (result),
        .cout(cout)
    );

    assign carry = cout ^ add_or_sub;

    assign zero = ~(|result);

    assign overflow=(a[data_len-1] == b_2_s_complement[data_len-1]) && (result[data_len-1] != a[data_len-1]);

endmodule
