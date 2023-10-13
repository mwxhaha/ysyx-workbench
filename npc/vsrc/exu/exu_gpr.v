`include "config.vh"
`include "inst.vh"

module exu_gpr (
    input wire clk,
    input wire rst,
    input wire [`INST_NUM_WIDTH-1:0] inst_num,
    input wire [`INST_TYPE_WIDTH-1:0] inst_type,
    input wire [`IMM_WIDTH-1:0] imm,
    input wire [`ISA_WIDTH-1:0] pc_out,
    input wire [`ISA_WIDTH-1:0] src1,
    input wire [`ISA_WIDTH-1:0] src2,
    output wire [`ISA_WIDTH-1:0] srd,
    output wire gpr_w_en,
    input wire [`ISA_WIDTH-1:0] mem_r,
    input wire [`ISA_WIDTH-1:0] alu_result
);

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_NUM_MAX),
        .KEY_LEN (`INST_NUM_WIDTH),
        .DATA_LEN(`ISA_WIDTH)
    ) muxkeywithdefault_srd (
        .out(srd),
        .key(inst_num),
        .default_out(`ISA_WIDTH'b0),
        .lut({
            `INST_NUM_WIDTH'd`auipc,
            alu_result,
            `INST_NUM_WIDTH'd`jal,
            alu_result,
            `INST_NUM_WIDTH'd`jalr,
            alu_result,
            `INST_NUM_WIDTH'd`beq,
            `ISA_WIDTH'b0,
            `INST_NUM_WIDTH'd`bne,
            `ISA_WIDTH'b0,
            `INST_NUM_WIDTH'd`lw,
            mem_r,
            `INST_NUM_WIDTH'd`sw,
            `ISA_WIDTH'b0,
            `INST_NUM_WIDTH'd`addi,
            alu_result,
            `INST_NUM_WIDTH'd`sltiu,
            alu_result,
            `INST_NUM_WIDTH'd`add,
            alu_result,
            `INST_NUM_WIDTH'd`sub,
            alu_result,
            `INST_NUM_WIDTH'd`sltu,
            alu_result,
            `INST_NUM_WIDTH'd`ixor,
            alu_result,
            `INST_NUM_WIDTH'd`ior,
            alu_result,
            `INST_NUM_WIDTH'd`ebreak,
            `ISA_WIDTH'b0
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_TYPE_MAX),
        .KEY_LEN (`INST_TYPE_WIDTH),
        .DATA_LEN(1)
    ) muxkeywithdefault_gpr_w_en (
        .out(gpr_w_en),
        .key(inst_type),
        .default_out(1'b0),
        .lut({
            `INST_TYPE_WIDTH'd`R,
            1'b1,
            `INST_TYPE_WIDTH'd`I,
            1'b1,
            `INST_TYPE_WIDTH'd`S,
            1'b0,
            `INST_TYPE_WIDTH'd`B,
            1'b0,
            `INST_TYPE_WIDTH'd`U,
            1'b1,
            `INST_TYPE_WIDTH'd`J,
            1'b1
        })

    );

endmodule
