`include "config.v"

module exu_alu
    (
        input wire clk,rst,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        input wire [`ISA_WIDTH-1:0] pc_out,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        input wire [`ISA_WIDTH-1:0] mem_r,
        input wire [`ISA_WIDTH-1:0] alu_result,
        output wire [`ISA_WIDTH-1:0] alu_a,
        output wire [`ISA_WIDTH-1:0] alu_b,
        output wire [`ALU_FUNC_WIDTH-1:0] alu_func
    );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_alu_a
        (
            .out(alu_a),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,pc_out,
                  `INST_NUM_WIDTH'd`jal,pc_out,
                  `INST_NUM_WIDTH'd`jalr,pc_out,
                  `INST_NUM_WIDTH'd`beq,src1,
                  `INST_NUM_WIDTH'd`lw,src1,
                  `INST_NUM_WIDTH'd`sw,src1,
                  `INST_NUM_WIDTH'd`addi,src1,
                  `INST_NUM_WIDTH'd`add,src1,
                  `INST_NUM_WIDTH'd`sub,src1,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_alu_b
        (
            .out(alu_b),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,imm,
                  `INST_NUM_WIDTH'd`jal,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`jalr,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`beq,src2,
                  `INST_NUM_WIDTH'd`lw,imm,
                  `INST_NUM_WIDTH'd`sw,imm,
                  `INST_NUM_WIDTH'd`addi,imm,
                  `INST_NUM_WIDTH'd`add,src2,
                  `INST_NUM_WIDTH'd`sub,src2,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ALU_FUNC_WIDTH)
        )
        muxkeywithdefault_alu_func
        (
            .out(alu_func),
            .key(inst_num),
            .default_out(`ALU_FUNC_WIDTH'd`NO_FUNC),
            .lut({`INST_NUM_WIDTH'd`auipc,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`jal,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`jalr,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`beq,`ALU_FUNC_WIDTH'd`EQ,
                  `INST_NUM_WIDTH'd`lw,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`sw,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`addi,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`add,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`sub,`ALU_FUNC_WIDTH'd`SUB_S,
                  `INST_NUM_WIDTH'd`ebreak,`ALU_FUNC_WIDTH'd`NO_FUNC})
        );

endmodule
