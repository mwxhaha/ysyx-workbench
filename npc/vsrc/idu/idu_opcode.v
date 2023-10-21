`include "config.vh"
`include "inst.vh"

module idu_opcode (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] inst,
    input wire [`INST_NUM_WIDTH-1:0] inst_beq_num,
    input wire [`INST_NUM_WIDTH-1:0] inst_lb_num,
    input wire [`INST_NUM_WIDTH-1:0] inst_sb_num,
    input wire [`INST_NUM_WIDTH-1:0] inst_addi_num,
    input wire [`INST_NUM_WIDTH-1:0] inst_add_num,
    input wire [`INST_NUM_WIDTH-1:0] inst_ebreak_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_num,
    output wire [`INST_TYPE_WIDTH-1:0] inst_type,
    output wire [`REG_ADDR_WIDTH-1:0] rd,
    output wire [`REG_ADDR_WIDTH-1:0] rs1,
    output wire [`REG_ADDR_WIDTH-1:0] rs2,
    output wire [`IMM_WIDTH-1:0] imm,
    output wire [`SHAMT_WIDTH-1:0] shamt
);

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_NUM_IDU_MAX),
        .KEY_LEN (`OPCODE_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_num (
        .out(inst_num),
        .key(inst[`OPCODE_WIDTH-1:0]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut({
            `OPCODE_WIDTH'b0110111,
            `INST_NUM_WIDTH'd`lui,
            `OPCODE_WIDTH'b0010111,
            `INST_NUM_WIDTH'd`auipc,
            `OPCODE_WIDTH'b1101111,
            `INST_NUM_WIDTH'd`jal,
            `OPCODE_WIDTH'b1100111,
            `INST_NUM_WIDTH'd`jalr,
            `OPCODE_WIDTH'b1100011,
            inst_beq_num,
            `OPCODE_WIDTH'b0000011,
            inst_lb_num,
            `OPCODE_WIDTH'b0100011,
            inst_sb_num,
            `OPCODE_WIDTH'b0010011,
            inst_addi_num,
            `OPCODE_WIDTH'b0110011,
            inst_add_num,
            `OPCODE_WIDTH'b1110011,
            inst_ebreak_num
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_NUM_IDU_MAX),
        .KEY_LEN (`OPCODE_WIDTH),
        .DATA_LEN(`INST_TYPE_WIDTH)
    ) muxkeywithdefault_inst_type (
        .out(inst_type),
        .key(inst[`OPCODE_WIDTH-1:0]),
        .default_out(`INST_TYPE_WIDTH'd`N),
        .lut({
            `OPCODE_WIDTH'b0110111,
            `INST_TYPE_WIDTH'd`U,
            `OPCODE_WIDTH'b0010111,
            `INST_TYPE_WIDTH'd`U,
            `OPCODE_WIDTH'b1101111,
            `INST_TYPE_WIDTH'd`J,
            `OPCODE_WIDTH'b1100111,
            `INST_TYPE_WIDTH'd`I,
            `OPCODE_WIDTH'b1100011,
            `INST_TYPE_WIDTH'd`B,
            `OPCODE_WIDTH'b0000011,
            `INST_TYPE_WIDTH'd`I,
            `OPCODE_WIDTH'b0100011,
            `INST_TYPE_WIDTH'd`S,
            `OPCODE_WIDTH'b0010011,
            `INST_TYPE_WIDTH'd`I,
            `OPCODE_WIDTH'b0110011,
            `INST_TYPE_WIDTH'd`R,
            `OPCODE_WIDTH'b1110011,
            `INST_TYPE_WIDTH'd`I
        })
    );

    assign rd  = inst[7+`REG_ADDR_WIDTH-1:7];
    assign rs1 = inst[15+`REG_ADDR_WIDTH-1:15];
    assign rs2 = inst[20+`REG_ADDR_WIDTH-1:20];

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_TYPE_MAX),
        .KEY_LEN (`INST_TYPE_WIDTH),
        .DATA_LEN(`IMM_WIDTH)
    ) muxkeywithdefault_imm (
        .out(imm),
        .key(inst_type),
        .default_out(`IMM_WIDTH'b0),
        .lut({
            `INST_TYPE_WIDTH'd`R,
            `IMM_WIDTH'b0,
            `INST_TYPE_WIDTH'd`I,
            {{20{inst[31]}}, inst[31:20]},
            `INST_TYPE_WIDTH'd`S,
            {{20{inst[31]}}, inst[31:25], inst[11:7]},
            `INST_TYPE_WIDTH'd`B,
            {{19{inst[31]}}, inst[31:31], inst[7:7], inst[30:25], inst[11:8], 1'b0},
            `INST_TYPE_WIDTH'd`U,
            {inst[31:12], {12{1'b0}}},
            `INST_TYPE_WIDTH'd`J,
            {{11{inst[31]}}, inst[31:31], inst[19:12], inst[20:20], inst[30:21], 1'b0}
        })
    );

    assign shamt = inst[20+`SHAMT_WIDTH-1:20];

endmodule
