module adder #(
    parameter data_len = 4
) (
    input  wire [data_len-1:0] a,
    input  wire [data_len-1:0] b,
    input  wire                cin,
    output wire [data_len-1:0] s,
    output wire                cout
);

    wire [data_len-2:0] c;

    genvar i;
    generate
        for (i = 1; i <= data_len - 2; i = i + 1) begin
            full_adder full_adder_i (
                .a   (a[i]),
                .b   (b[i]),
                .cin (c[i-1]),
                .s   (s[i]),
                .cout(c[i])
            );
        end
    endgenerate

    full_adder full_adder_1 (
        .a   (a[0]),
        .b   (b[0]),
        .cin (cin),
        .s   (s[0]),
        .cout(c[0])
    );

    full_adder full_adder_2 (
        .a   (a[data_len-1]),
        .b   (b[data_len-1]),
        .cin (c[data_len-2]),
        .s   (s[data_len-1]),
        .cout(cout)
    );

endmodule
