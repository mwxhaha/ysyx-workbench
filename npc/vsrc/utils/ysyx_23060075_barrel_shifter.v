module ysyx_23060075_barrel_shifter #(
    parameter data_len = 32
) (
    input  wire [data_len-1:0] din,
    input  wire [         4:0] shamt,
    input  wire                is_right,
    input  wire                is_algorism,
    output wire [data_len-1:0] dout
);

    wire sign = is_algorism & din[data_len-1];

    wire [data_len-1:0] d_1;
    wire [data_len-1:0] d_2;
    wire [data_len-1:0] d_3;
    wire [data_len-1:0] d_4;

    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) mux_d_1 (
        .out(d_1),
        .key({shamt[0], is_right}),
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

    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) mux_d_2 (
        .out(d_2),
        .key({shamt[1], is_right}),
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

    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) mux_d_3 (
        .out(d_3),
        .key({shamt[2], is_right}),
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

    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) mux_d_4 (
        .out(d_4),
        .key({shamt[3], is_right}),
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

    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(data_len)
    ) mux_d_5 (
        .out(dout),
        .key({shamt[4], is_right}),
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
