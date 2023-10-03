module idu
    #(
         parameter ISA_WIDTH = 32,
         parameter INSTRUCTION_NUMBER_WIDTH = 8,
         parameter INSTRUCTION_TYPE_WIDTH = 3,
         parameter REGISTER_NUMBER_WIDTH = 5,
         parameter OPCODE_WIDTH = 7
     )
     (
         input wire clk,rst,
         input wire [ISA_WIDTH-1:0] instruction,
         output wire [INSTRUCTION_TYPE_WIDTH-1:0] instruction_type,
         output wire [INSTRUCTION_NUMBER_WIDTH-1:0] instruction_number,
         output wire [REGISTER_NUMBER_WIDTH-1:0] rd,rs1,rs2,
         output wire [ISA_WIDTH-1:0] imm
     );

    MuxKeyWithDefault
        #(
            .NR_KEY(2),
            .KEY_LEN(OPCODE_WIDTH),
            .DATA_LEN(INSTRUCTION_NUMBER_WIDTH)
        )
        muxkeywithdefault_instruction_number
        (
            .out(instruction_number),
            .key(instruction[OPCODE_WIDTH-1:0]),
            .default_out(0),
            .lut({7'b0010111,8'd2,
                  7'b1110011,8'd41})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(2),
            .KEY_LEN(OPCODE_WIDTH),
            .DATA_LEN(INSTRUCTION_TYPE_WIDTH)
        )
        muxkeywithdefault_instruction_type
        (
            .out(instruction_type),
            .key(instruction[OPCODE_WIDTH-1:0]),
            .default_out(0),
            .lut({7'b0010111,3'd4,
                  7'b1110011,3'd1})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(2),
            .KEY_LEN(INSTRUCTION_TYPE_WIDTH),
            .DATA_LEN(ISA_WIDTH)
        )
        muxkeywithdefault_imm
        (
            .out(imm),
            .key(instruction_type),
            .default_out(0),
            .lut({3'd1,{{20{instruction[31]}},instruction[31:20]},
                 3'd4,{instruction[31:12],{12{1'b0}}}})
        );

    assign rd=instruction[11:7];
    assign rs1=instruction[19:15];
    assign rs2=instruction[24:20];

endmodule
