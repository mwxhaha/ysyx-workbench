module barrel_shifter #(
    parameter data_len = 32
) (
    input wire [data_len-1:0] din,
    input wire [5:0] shamt,
    input wire left_or_right,
    input wire algorism_or_logic,
    output wire [data_len-1:0] dout
);

    wire sign = algorism_or_logic ? din[data_len-1] : 1'b0;

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
        .key({left_or_right, shamt[0]}),
        .lut({
            2'b00,
            din,
            2'b01,
            {sign, din[data_len-1:1]},
            2'b10,
            din,
            2'b11,
            {din[data_len-2:0], 1'b0}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_2 (
        .out(d_2),
        .key({left_or_right, shamt[1]}),
        .lut({
            2'b00,
            d_1,
            2'b01,
            {{2{sign}}, d_1[data_len-1:2]},
            2'b10,
            d_1,
            2'b11,
            {d_1[data_len-3:0], {2{1'b0}}}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_3 (
        .out(d_3),
        .key({left_or_right, shamt[2]}),
        .lut({
            2'b00,
            d_2,
            2'b01,
            {{4{sign}}, d_2[data_len-1:4]},
            2'b10,
            d_2,
            2'b11,
            {d_2[data_len-5:0], {4{1'b0}}}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_4 (
        .out(d_4),
        .key({left_or_right, shamt[3]}),
        .lut({
            2'b00,
            d_3,
            2'b01,
            {{8{sign}}, d_3[data_len-1:8]},
            2'b10,
            d_3,
            2'b11,
            {d_3[data_len-9:0], {8{1'b0}}}
        })
    );

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) muxkey_d_5 (
        .out(dout),
        .key({left_or_right, shamt[4]}),
        .lut({
            2'b00,
            d_4,
            2'b01,
            {{16{sign}}, d_4[data_len-1:16]},
            2'b10,
            d_4,
            2'b11,
            {d_4[data_len-17:0], {16{1'b0}}}
        })
    );

endmodule
