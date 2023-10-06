`include "config.v"

module alu
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] a,b,
        input wire [`ALU_FUNC_WIDTH-1:0] func,
        output wire [`ISA_WIDTH-1:0] result
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
            .key         	( func          ),
            .default_out 	( 1'b0  ),
            .lut ({`ALU_FUNC_WIDTH'd`ADD_S,1'b0,
                   `ALU_FUNC_WIDTH'd`SUB_S,1'b1,
                   `ALU_FUNC_WIDTH'd`ADD_U,1'b0,
                   `ALU_FUNC_WIDTH'd`SUB_U,1'b1,
                   `ALU_FUNC_WIDTH'd`NOT,1'b0,
                   `ALU_FUNC_WIDTH'd`AND,1'b0,
                   `ALU_FUNC_WIDTH'd`OR,1'b0,
                   `ALU_FUNC_WIDTH'd`XOR,1'b0})
        );

    wire [`ISA_WIDTH-1:0] adder_suber_1_result;
    wire carry,zero,overflow;
    adder_suber
        #(
            .data_len(`ISA_WIDTH)
        )
        adder_suber_1
        (
            .a        	( a                     ),
            .b        	( b                     ),
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
            .out         	( result          ),
            .key         	( func          ),
            .default_out 	( `ISA_WIDTH'b0 ),
            .lut ({`ALU_FUNC_WIDTH'd`ADD_S,adder_suber_1_result,
                   `ALU_FUNC_WIDTH'd`SUB_S,adder_suber_1_result,
                   `ALU_FUNC_WIDTH'd`ADD_U,adder_suber_1_result,
                   `ALU_FUNC_WIDTH'd`SUB_U,adder_suber_1_result,
                   `ALU_FUNC_WIDTH'd`NOT,~a,
                   `ALU_FUNC_WIDTH'd`AND,a&b,
                   `ALU_FUNC_WIDTH'd`OR,a|b,
                   `ALU_FUNC_WIDTH'd`XOR,a^b})
        );

endmodule
