`include "vsrc/config.v"
module idu
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] inst,
        output wire [`INST_TYPE_WIDTH-1:0] inst_type,
        output wire [`INST_NUM_WIDTH-1:0] inst_num,
        output wire [`REG_ADDR_WIDTH-1:0] rd,rs1,rs2,
        output wire [`IMM_WIDTH-1:0] imm
    );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`OPCODE_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_num
        (
            .out(inst_num),
            .key(inst[`OPCODE_WIDTH-1:0]),
            .default_out(`inv),
            .lut({`OPCODE_WIDTH'b0010111,`INST_NUM_WIDTH'd`auipc,
                  `OPCODE_WIDTH'b1110011,`INST_NUM_WIDTH'd`ebreak})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`OPCODE_WIDTH),
            .DATA_LEN(`INST_TYPE_WIDTH)
        )
        muxkeywithdefault_inst_type
        (
            .out(inst_type),
            .key(inst[`OPCODE_WIDTH-1:0]),
            .default_out(`N),
            .lut({`OPCODE_WIDTH'b0010111,`INST_TYPE_WIDTH'd`U,
                  `OPCODE_WIDTH'b1110011,`INST_TYPE_WIDTH'd`I})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(`IMM_WIDTH)
        )
        muxkeywithdefault_imm
        (
            .out(imm),
            .key(inst_type),
            .default_out(0),
            .lut({`INST_TYPE_WIDTH'd`I,{{20{inst[31]}},inst[31:20]},
                  `INST_TYPE_WIDTH'd`U,{inst[31:12],{12{1'b0}}}})
        );

    assign rd=inst[11:7];
    assign rs1=inst[19:15];
    assign rs2=inst[24:20];

endmodule
