`include "config.vh"

module alu (
    input  wire [      `ISA_WIDTH-1:0] alu_a,
    input  wire [      `ISA_WIDTH-1:0] alu_b,
    input  wire [`ALU_FUNCT_WIDTH-1:0] alu_funct,
    output wire [      `ISA_WIDTH-1:0] alu_result
);

    wire                  is_sub = alu_funct[3];
    wire [`ISA_WIDTH-1:0] adder_suber_alu_result;
    wire                  carry;
    wire                  overflow;
    
    adder_suber #(
        .data_len(`ISA_WIDTH)
    ) adder_suber_alu (
        .a       (alu_a),
        .b       (alu_b),
        .is_sub  (is_sub),
        .result  (adder_suber_alu_result),
        .carry   (carry),
        .overflow(overflow)
    );

    wire is_right = alu_funct[2];
    wire is_algorism = alu_funct[3];
    wire [`ISA_WIDTH-1:0] barrel_shifter_alu_result;

    barrel_shifter #(
        .data_len(`ISA_WIDTH)
    ) barrel_shifter_alu (
        .din        (alu_a),
        .shamt      (alu_b[`SHAMT_WIDTH-1:0]),
        .is_right   (is_right),
        .is_algorism(is_algorism),
        .dout       (barrel_shifter_alu_result)
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
            adder_suber_alu_result,
            `ALU_FUNCT_WIDTH'd`SUB,
            adder_suber_alu_result,
            `ALU_FUNCT_WIDTH'd`LT,
            {{`ISA_WIDTH - 1{1'b0}}, adder_suber_alu_result[`ISA_WIDTH-1] ^ overflow},
            `ALU_FUNCT_WIDTH'd`LTU,
            {{`ISA_WIDTH - 1{1'b0}}, carry},
            `ALU_FUNCT_WIDTH'd`SLL,
            barrel_shifter_alu_result,
            `ALU_FUNCT_WIDTH'd`SRL,
            barrel_shifter_alu_result,
            `ALU_FUNCT_WIDTH'd`SRA,
            barrel_shifter_alu_result,
            `ALU_FUNCT_WIDTH'd`XOR,
            alu_a ^ alu_b,
            `ALU_FUNCT_WIDTH'd`OR,
            alu_a | alu_b,
            `ALU_FUNCT_WIDTH'd`AND,
            alu_a & alu_b,
            `ALU_FUNCT_WIDTH'd`EQ,
            {{`ISA_WIDTH - 1{1'b0}}, ~(|(alu_a ^ alu_b))},
            `ALU_FUNCT_WIDTH'd`NE,
            {{`ISA_WIDTH - 1{1'b0}}, |(alu_a ^ alu_b)},
            `ALU_FUNCT_WIDTH'd`GE,
            {{`ISA_WIDTH - 1{1'b0}}, adder_suber_alu_result[`ISA_WIDTH-1] ~^ overflow},
            `ALU_FUNCT_WIDTH'd`GEU,
            {{`ISA_WIDTH - 1{1'b0}}, ~carry}
        })
    );

endmodule
