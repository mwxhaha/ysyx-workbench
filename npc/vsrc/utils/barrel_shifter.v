module barrel_shifter #(
    parameter data_len = 32
) (
    input wire [data_len-1:0] din,
    input wire [4:0] shamt,
    input wire left_or_right,
    input wire algorism_or_logic,
    output wire [data_len-1:0] dout
);

    wire sign = ~algorism_or_logic & din[data_len-1];

    wire [data_len-1:0] d_1;
    wire [data_len-1:0] d_2;
    wire [data_len-1:0] d_3;
    wire [data_len-1:0] d_4;

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_1 (
        .out(d_1),
        .key({shamt[0], left_or_right}),
        .lut({
            2'b00,
            din,
            2'b01,
            din,
            2'b10,
            {din[data_len-2:0], 1'b0},
            2'b11,
            {sign, din[data_len-1:1]}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_2 (
        .out(d_2),
        .key({shamt[1], left_or_right}),
        .lut({
            2'b00,
            d_1,
            2'b01,
            d_1,
            2'b10,
            {d_1[data_len-3:0], {2{1'b0}}},
            2'b11,
            {{2{sign}}, d_1[data_len-1:2]}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_3 (
        .out(d_3),
        .key({shamt[2], left_or_right}),
        .lut({
            2'b00,
            d_2,
            2'b01,
            d_2,
            2'b10,
            {d_2[data_len-5:0], {4{1'b0}}},
            2'b11,
            {{4{sign}}, d_2[data_len-1:4]}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_4 (
        .out(d_4),
        .key({shamt[3], left_or_right}),
        .lut({
            2'b00,
            d_3,
            2'b01,
            d_3,
            2'b10,
            {d_3[data_len-9:0], {8{1'b0}}},
            2'b11,
            {{8{sign}}, d_3[data_len-1:8]}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_5 (
        .out(dout),
        .key({shamt[4], left_or_right}),
        .lut({
            2'b00,
            d_4,
            2'b01,
            d_4,
            2'b10,
            {d_4[data_len-17:0], {16{1'b0}}},
            2'b11,
            {{16{sign}}, d_4[data_len-1:16]}
        })
    );

endmodule
