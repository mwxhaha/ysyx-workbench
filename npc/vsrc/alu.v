`include "config.vh"
`include "inst.vh"

module alu (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] alu_a,
    input wire [`ISA_WIDTH-1:0] alu_b,
    input wire [`ALU_FUNCT_WIDTH-1:0] alu_funct,
    output wire [`ISA_WIDTH-1:0] alu_result
);

    wire add_or_sub;
    MuxKeyWithDefault #(
        .NR_KEY  (`ALU_FUNCT_MAX),
        .KEY_LEN (`ALU_FUNCT_WIDTH),
        .DATA_LEN(1)
    ) muxkeywithdefault_add_or_sub (
        .out(add_or_sub),
        .key(alu_funct),
        .default_out(1'b0),
        .lut({
            `ALU_FUNCT_WIDTH'd`ADD,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB,
            1'b1,
            `ALU_FUNCT_WIDTH'd`ADD_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB_U,
            1'b1,
            `ALU_FUNCT_WIDTH'd`NOT,
            1'b0,
            `ALU_FUNCT_WIDTH'd`AND,
            1'b0,
            `ALU_FUNCT_WIDTH'd`OR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`XOR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`EQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`NEQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`LESS_U,
            1'b1,
            `ALU_FUNCT_WIDTH'd`SHIFT_L_L,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SHIFT_R_A,
            1'b0
        })
    );

    wire [`ISA_WIDTH-1:0] adder_suber_1_result;
    wire carry, zero, overflow;
    adder_suber #(
        .data_len(`ISA_WIDTH)
    ) adder_suber_1 (
        .a         (alu_a),
        .b         (alu_b),
        .add_or_sub(add_or_sub),
        .result    (adder_suber_1_result),
        .carry     (carry),
        .zero      (zero),
        .overflow  (overflow)
    );

    wire left_or_right;
    MuxKeyWithDefault #(
        .NR_KEY  (`ALU_FUNCT_MAX),
        .KEY_LEN (`ALU_FUNCT_WIDTH),
        .DATA_LEN(1)
    ) muxkeywithdefault_left_or_right (
        .out(left_or_right),
        .key(alu_funct),
        .default_out(1'b0),
        .lut({
            `ALU_FUNCT_WIDTH'd`ADD,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB,
            1'b0,
            `ALU_FUNCT_WIDTH'd`ADD_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`NOT,
            1'b0,
            `ALU_FUNCT_WIDTH'd`AND,
            1'b0,
            `ALU_FUNCT_WIDTH'd`OR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`XOR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`EQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`NEQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`LESS_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SHIFT_L_L,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SHIFT_R_A,
            1'b1
        })
    );

    wire algorism_or_logic;
    MuxKeyWithDefault #(
        .NR_KEY  (`ALU_FUNCT_MAX),
        .KEY_LEN (`ALU_FUNCT_WIDTH),
        .DATA_LEN(1)
    ) muxkeywithdefault_algorism_or_logic (
        .out(algorism_or_logic),
        .key(alu_funct),
        .default_out(1'b0),
        .lut({
            `ALU_FUNCT_WIDTH'd`ADD,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB,
            1'b0,
            `ALU_FUNCT_WIDTH'd`ADD_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SUB_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`NOT,
            1'b0,
            `ALU_FUNCT_WIDTH'd`AND,
            1'b0,
            `ALU_FUNCT_WIDTH'd`OR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`XOR,
            1'b0,
            `ALU_FUNCT_WIDTH'd`EQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`NEQ,
            1'b0,
            `ALU_FUNCT_WIDTH'd`LESS_U,
            1'b0,
            `ALU_FUNCT_WIDTH'd`SHIFT_L_L,
            1'b1,
            `ALU_FUNCT_WIDTH'd`SHIFT_R_A,
            1'b0
        })
    );

    wire [`ISA_WIDTH-1:0] barrel_shifter_1_result;
    barrel_shifter #(
        .data_len(`ISA_WIDTH)
    ) barrel_shifter_1 (
        .din(alu_a),
        .shamt(alu_b[5:0]),
        .left_or_right(left_or_right),
        .algorism_or_logic(algorism_or_logic),
        .dout(barrel_shifter_1_result)
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`ALU_FUNCT_MAX),
        .KEY_LEN (`ALU_FUNCT_WIDTH),
        .DATA_LEN(`ISA_WIDTH)
    ) muxkeywithdefault_result (
        .out(alu_result),
        .key(alu_funct),
        .default_out(`ISA_WIDTH'b0),
        .lut({
            `ALU_FUNCT_WIDTH'd`ADD,
            adder_suber_1_result,
            `ALU_FUNCT_WIDTH'd`SUB,
            adder_suber_1_result,
            `ALU_FUNCT_WIDTH'd`ADD_U,
            adder_suber_1_result,
            `ALU_FUNCT_WIDTH'd`SUB_U,
            adder_suber_1_result,
            `ALU_FUNCT_WIDTH'd`NOT,
            ~alu_a,
            `ALU_FUNCT_WIDTH'd`AND,
            alu_a & alu_b,
            `ALU_FUNCT_WIDTH'd`OR,
            alu_a | alu_b,
            `ALU_FUNCT_WIDTH'd`XOR,
            alu_a ^ alu_b,
            `ALU_FUNCT_WIDTH'd`EQ,
            {{`ISA_WIDTH - 1{1'b0}}, ~(|(alu_a ^ alu_b))},
            `ALU_FUNCT_WIDTH'd`NEQ,
            {{`ISA_WIDTH - 1{1'b0}}, |(alu_a ^ alu_b)},
            `ALU_FUNCT_WIDTH'd`LESS_U,
            {{`ISA_WIDTH - 1{1'b0}}, carry},
            `ALU_FUNCT_WIDTH'd`SHIFT_L_L,
            barrel_shifter_1_result,
            `ALU_FUNCT_WIDTH'd`SHIFT_R_A,
            barrel_shifter_1_result
        })
    );

endmodule
