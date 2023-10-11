`include "config.v"

module exu_pc
    (
        input wire clk,rst,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        input wire [`ISA_WIDTH-1:0] pc_out,
        output wire [`ISA_WIDTH-1:0] pc_in,
        output wire pc_w_en,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        input wire [`ISA_WIDTH-1:0] mem_r,
        input wire [`ISA_WIDTH-1:0] alu_result
    );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_adder_pc_a
        (
            .out(adder_pc_a),
            .key(inst_num),
            .default_out(pc_out),
            .lut({`INST_NUM_WIDTH'd`auipc,pc_out,
                  `INST_NUM_WIDTH'd`jal,pc_out,
                  `INST_NUM_WIDTH'd`jalr,src1,
                  `INST_NUM_WIDTH'd`beq,pc_out,
                  `INST_NUM_WIDTH'd`lw,pc_out,
                  `INST_NUM_WIDTH'd`sw,pc_out,
                  `INST_NUM_WIDTH'd`addi,pc_out,
                  `INST_NUM_WIDTH'd`add,pc_out,
                  `INST_NUM_WIDTH'd`sub,pc_out,
                  `INST_NUM_WIDTH'd`ebreak,pc_out})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_adder_pc_b
        (
            .out(adder_pc_b),
            .key(inst_num),
            .default_out(`ISA_WIDTH'd4),
            .lut({`INST_NUM_WIDTH'd`auipc,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`jal,imm,
                  `INST_NUM_WIDTH'd`jalr,imm,
                  `INST_NUM_WIDTH'd`beq,({`ISA_WIDTH{alu_result[0]}}&imm)|({`ISA_WIDTH{~alu_result[0]}}&`ISA_WIDTH'd4),
                  `INST_NUM_WIDTH'd`lw,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`sw,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`addi,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`add,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`sub,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'd4})
        );

    wire [`ISA_WIDTH-1:0] 	adder_pc_a;
    wire [`ISA_WIDTH-1:0] 	adder_pc_b;
    wire [`ISA_WIDTH-1:0] 	adder_pc_s;
    wire adder_pc_cout;
    adder
        #(
            .data_len(`ISA_WIDTH)
        )
        adder_pc
        (
            .a    	( adder_pc_a     ),
            .b    	( adder_pc_b     ),
            .cin  	( 1'b0   ),
            .s    	( adder_pc_s ),
            .cout 	( adder_pc_cout  )
        );

    wire is_not_jalr=|(inst_num^`INST_NUM_WIDTH'd`jalr);
    assign pc_in={adder_pc_s[`ISA_WIDTH-1:1],is_not_jalr&adder_pc_s[0]};

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_pc_w_en
        (
            .out(pc_w_en),
            .key(inst_type),
            .default_out(1'b0),
            .lut({`INST_TYPE_WIDTH'd`R,1'b1,
                  `INST_TYPE_WIDTH'd`I,1'b1,
                  `INST_TYPE_WIDTH'd`S,1'b1,
                  `INST_TYPE_WIDTH'd`B,1'b1,
                  `INST_TYPE_WIDTH'd`U,1'b1,
                  `INST_TYPE_WIDTH'd`J,1'b1})
        );

endmodule
