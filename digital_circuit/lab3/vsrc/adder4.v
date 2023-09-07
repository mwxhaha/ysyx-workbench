module adder4
    (
        input wire [3:0] a,b,
        output wire [3:0] s
    );

    adder
        #(
            .data_len(4)
        )
        adder_i
        (
            .a(a),
            .b(b),
            .cin(1'b0),
            .s(s),
            .cout()
        );

endmodule
