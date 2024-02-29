`include "config.vh"

module ysyx_23060075_alu (
    input wire [      `ysyx_23060075_ISA_WIDTH-1:0] alu_a,
    input wire [      `ysyx_23060075_ISA_WIDTH-1:0] alu_b,
    input wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] alu_result
);

    wire                                is_sub = alu_funct[3];
    wire [`ysyx_23060075_ISA_WIDTH-1:0] adder_alu_alu_result;
    wire                                carry;
    wire                                overflow;
    ysyx_23060075_adder_alu #(
        .data_len(`ysyx_23060075_ISA_WIDTH)
    ) adder_alu_alu (
        .a       (alu_a),
        .b       (alu_b),
        .is_sub  (is_sub),
        .result  (adder_alu_alu_result),
        .carry   (carry),
        .overflow(overflow)
    );

    wire                                is_right = alu_funct[2];
    wire                                is_algorism = alu_funct[3];
    wire [`ysyx_23060075_ISA_WIDTH-1:0] barrel_shifter_alu_result;
    ysyx_23060075_barrel_shifter #(
        .data_len(`ysyx_23060075_ISA_WIDTH)
    ) barrel_shifter_alu (
        .din        (alu_a),
        .shamt      (alu_b[`ysyx_23060075_SHAMT_WIDTH-1:0]),
        .is_right   (is_right),
        .is_algorism(is_algorism),
        .dout       (barrel_shifter_alu_result)
    );

    ysyx_23060075_mux_def #(
        .NR_KEY  (14),
        .KEY_LEN (`ysyx_23060075_ALU_FUNCT_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_result (
        .out(alu_result),
        .key(alu_funct),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_ADD,
            adder_alu_alu_result,
            `ysyx_23060075_SUB,
            adder_alu_alu_result,
            `ysyx_23060075_LT,
            {
                {`ysyx_23060075_ISA_WIDTH - 1{1'b0}},
                adder_alu_alu_result[`ysyx_23060075_ISA_WIDTH-1] ^ overflow
            },
            `ysyx_23060075_LTU,
            {{`ysyx_23060075_ISA_WIDTH - 1{1'b0}}, carry},
            `ysyx_23060075_XOR,
            alu_a ^ alu_b,
            `ysyx_23060075_OR,
            alu_a | alu_b,
            `ysyx_23060075_AND,
            alu_a & alu_b,
            `ysyx_23060075_SLL,
            barrel_shifter_alu_result,
            `ysyx_23060075_SRL,
            barrel_shifter_alu_result,
            `ysyx_23060075_SRA,
            barrel_shifter_alu_result,
            `ysyx_23060075_EQ,
            {{`ysyx_23060075_ISA_WIDTH - 1{1'b0}}, ~(|(alu_a ^ alu_b))},
            `ysyx_23060075_NE,
            {{`ysyx_23060075_ISA_WIDTH - 1{1'b0}}, |(alu_a ^ alu_b)},
            `ysyx_23060075_GE,
            {
                {`ysyx_23060075_ISA_WIDTH - 1{1'b0}},
                adder_alu_alu_result[`ysyx_23060075_ISA_WIDTH-1] ~^ overflow
            },
            `ysyx_23060075_GEU,
            {{`ysyx_23060075_ISA_WIDTH - 1{1'b0}}, ~carry}
        })
    );

endmodule
