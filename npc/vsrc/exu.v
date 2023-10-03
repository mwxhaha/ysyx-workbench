`include "vsrc/config.v"

module exu
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] pc,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        output wire [`ISA_WIDTH-1:0] srd,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        output wire gpr_wen
    );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_srd
        (
            .out(srd),
            .key(inst_num),
            .default_out(0),
            .lut({`INST_NUM_WIDTH'd`auipc,pc+imm,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_gpr_wen
        (
            .out(gpr_wen),
            .key(inst_type),
            .default_out(0),
            .lut({`INST_TYPE_WIDTH'd`I,1'b1,
                  `INST_TYPE_WIDTH'd`U,1'b1})
        );

endmodule
