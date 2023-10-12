`include "config.v"

module alu
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] alu_a,alu_b,
        input wire [`ALU_FUNC_WIDTH-1:0] alu_func,
        output wire [`ISA_WIDTH-1:0] alu_result
    );

    wire add_or_sub;
    MuxKeyWithDefault
        #(
            .NR_KEY(`ALU_FUNC_MAX),
            .KEY_LEN(`ALU_FUNC_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_add_or_sub
        (
            .out         	( add_or_sub          ),
            .key         	( alu_func          ),
            .default_out 	( 1'b0  ),
            .lut ({
                      `ALU_FUNC_WIDTH'd`ADD,1'b0,
                      `ALU_FUNC_WIDTH'd`SUB,1'b1,
                      `ALU_FUNC_WIDTH'd`ADD_U,1'b0,
                      `ALU_FUNC_WIDTH'd`SUB_U,1'b1,
                      `ALU_FUNC_WIDTH'd`NOT,1'b0,
                      `ALU_FUNC_WIDTH'd`AND,1'b0,
                      `ALU_FUNC_WIDTH'd`OR,1'b0,
                      `ALU_FUNC_WIDTH'd`XOR,1'b0,
                      `ALU_FUNC_WIDTH'd`EQ,1'b0,
                      `ALU_FUNC_WIDTH'd`NE,1'b0,
                      `ALU_FUNC_WIDTH'd`LESS_U,1'b1
                  })
        );

    wire [`ISA_WIDTH-1:0] adder_suber_1_result;
    wire carry,zero,overflow;
    adder_suber
        #(
            .data_len(`ISA_WIDTH)
        )
        adder_suber_1
        (
            .a        	( alu_a                     ),
            .b        	( alu_b                     ),
            .add_or_sub ( add_or_sub            ),
            .result   	( adder_suber_1_result  ),
            .carry    	( carry                 ),
            .zero     	( zero                  ),
            .overflow 	( overflow              )
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`ALU_FUNC_MAX),
            .KEY_LEN(`ALU_FUNC_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_result
        (
            .out         	( alu_result          ),
            .key         	( alu_func          ),
            .default_out 	( `ISA_WIDTH'b0 ),
            .lut ({
                      `ALU_FUNC_WIDTH'd`ADD,adder_suber_1_result,
                      `ALU_FUNC_WIDTH'd`SUB,adder_suber_1_result,
                      `ALU_FUNC_WIDTH'd`ADD_U,adder_suber_1_result,
                      `ALU_FUNC_WIDTH'd`SUB_U,adder_suber_1_result,
                      `ALU_FUNC_WIDTH'd`NOT,~alu_a,
                      `ALU_FUNC_WIDTH'd`AND,alu_a&alu_b,
                      `ALU_FUNC_WIDTH'd`OR,alu_a|alu_b,
                      `ALU_FUNC_WIDTH'd`XOR,alu_a^alu_b,
                      `ALU_FUNC_WIDTH'd`EQ,{{`ISA_WIDTH-1{1'b0}},~(|(alu_a^alu_b))},
                      `ALU_FUNC_WIDTH'd`NE,{{`ISA_WIDTH-1{1'b0}},|(alu_a^alu_b)},
                      `ALU_FUNC_WIDTH'd`LESS_U,{{`ISA_WIDTH-1{1'b0}},carry}
                  })
        );

endmodule
