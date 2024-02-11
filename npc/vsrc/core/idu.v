`include "config.vh"

module idu (
    input  wire                        clk,
    input  wire [      `ISA_WIDTH-1:0] inst,
    output wire [`INST_TYPE_WIDTH-1:0] inst_type,
    output wire [      `IMM_WIDTH-1:0] imm,
    output wire [   `OPCODE_WIDTH-1:0] opcode,
    output wire [   `FUNCT3_WIDTH-1:0] funct3,
    output wire [   `FUNCT7_WIDTH-1:0] funct7,
    input  wire [      `ISA_WIDTH-1:0] srd,
    output wire [      `ISA_WIDTH-1:0] src1,
    output wire [      `ISA_WIDTH-1:0] src2,
    input  wire                        gpr_w_en
);

    mux_def #(
        .NR_KEY  (`OPCODE_NUMBER_MAX),
        .KEY_LEN (`OPCODE_WIDTH),
        .DATA_LEN(`INST_TYPE_WIDTH)
    ) mux_def_inst_type (
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

    mux_def #(
        .NR_KEY  (`INST_TYPE_MAX),
        .KEY_LEN (`INST_TYPE_WIDTH),
        .DATA_LEN(`IMM_WIDTH)
    ) mux_def_imm (
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

    wire [`REG_ADDR_WIDTH-1:0] rd = inst[7+`REG_ADDR_WIDTH-1:7];
    wire [`REG_ADDR_WIDTH-1:0] rs1 = inst[15+`REG_ADDR_WIDTH-1:15];
    wire [`REG_ADDR_WIDTH-1:0] rs2 = inst[20+`REG_ADDR_WIDTH-1:20];
    assign opcode = inst[`OPCODE_WIDTH-1:0];
    assign funct3 = inst[12+`FUNCT3_WIDTH-1:12];
    assign funct7 = inst[25+`FUNCT7_WIDTH-1:25];

    gpr gpr_1 (
        .clk       (clk),
        .gpr_w     (srd),
        .gpr_1_r   (src1),
        .gpr_2_r   (src2),
        .gpr_w_addr(rd),
        .gpr_1_addr(rs1),
        .gpr_2_addr(rs2),
        .gpr_w_en  (gpr_w_en)
    );

endmodule
