`include "config.vh"

module ysyx_23060075_idu (
    input  wire                                      clk,
    input  wire                                      rst,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] inst,
    output wire [`ysyx_23060075_INST_TYPE_WIDTH-1:0] inst_type,
    output wire [      `ysyx_23060075_IMM_WIDTH-1:0] imm,
    output wire [   `ysyx_23060075_OPCODE_WIDTH-1:0] opcode,
    output wire [   `ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    output wire [   `ysyx_23060075_FUNCT7_WIDTH-1:0] funct7,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] srd,
    output wire [      `ysyx_23060075_ISA_WIDTH-1:0] src1,
    output wire [      `ysyx_23060075_ISA_WIDTH-1:0] src2,
    input  wire                                      gpr_w_en,
    input  wire                                      is_csri,
    input  wire                                      csr_w_en
);

    ysyx_23060075_mux_def #(
        .NR_KEY  (`ysyx_23060075_OPCODE_NUMBER_MAX),
        .KEY_LEN (`ysyx_23060075_OPCODE_WIDTH),
        .DATA_LEN(`ysyx_23060075_INST_TYPE_WIDTH)
    ) mux_def_inst_type (
        .out(inst_type),
        .key(inst[`ysyx_23060075_OPCODE_WIDTH-1:0]),
        .default_out(`ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_N),
        .lut({
            `ysyx_23060075_OPCODE_WIDTH'b0110111,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_U,
            `ysyx_23060075_OPCODE_WIDTH'b0010111,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_U,
            `ysyx_23060075_OPCODE_WIDTH'b1101111,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_J,
            `ysyx_23060075_OPCODE_WIDTH'b1100111,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I,
            `ysyx_23060075_OPCODE_WIDTH'b1100011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_B,
            `ysyx_23060075_OPCODE_WIDTH'b0000011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I,
            `ysyx_23060075_OPCODE_WIDTH'b0100011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_S,
            `ysyx_23060075_OPCODE_WIDTH'b0010011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I,
            `ysyx_23060075_OPCODE_WIDTH'b0110011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_R,
            `ysyx_23060075_OPCODE_WIDTH'b1110011,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I
        })
    );

    ysyx_23060075_mux_def #(
        .NR_KEY  (`ysyx_23060075_INST_TYPE_MAX),
        .KEY_LEN (`ysyx_23060075_INST_TYPE_WIDTH),
        .DATA_LEN(`ysyx_23060075_IMM_WIDTH)
    ) mux_def_imm (
        .out(imm),
        .key(inst_type),
        .default_out(`ysyx_23060075_IMM_WIDTH'b0),
        .lut({
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_R,
            `ysyx_23060075_IMM_WIDTH'b0,
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I,
            {{20{inst[31]}}, inst[31:20]},
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_S,
            {{20{inst[31]}}, inst[31:25], inst[11:7]},
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_B,
            {{19{inst[31]}}, inst[31:31], inst[7:7], inst[30:25], inst[11:8], 1'b0},
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_U,
            {inst[31:12], {12{1'b0}}},
            `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_J,
            {{11{inst[31]}}, inst[31:31], inst[19:12], inst[20:20], inst[30:21], 1'b0}
        })
    );

    wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] rd = inst[7+`ysyx_23060075_REG_ADDR_WIDTH-1:7];
    wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] rs1 = inst[15+`ysyx_23060075_REG_ADDR_WIDTH-1:15];
    wire [`ysyx_23060075_REG_ADDR_WIDTH-1:0] rs2 = inst[20+`ysyx_23060075_REG_ADDR_WIDTH-1:20];
    assign opcode = inst[`ysyx_23060075_OPCODE_WIDTH-1:0];
    assign funct3 = inst[12+`ysyx_23060075_FUNCT3_WIDTH-1:12];
    assign funct7 = inst[25+`ysyx_23060075_FUNCT7_WIDTH-1:25];
    wire [`ysyx_23060075_CSR_ADDR_WIDTH-1:0] csr_addr = inst[20+`ysyx_23060075_CSR_ADDR_WIDTH-1:20];
    wire [`ysyx_23060075_ISA_WIDTH-1:0] zimm = {{27'b0}, inst[19:15]};

    ysyx_23060075_gpr gpr_1 (
        .clk       (clk),
        .rst       (rst),
        .gpr_w     (csr_w_en ? csr_r : srd),
        .gpr_1_r   (src1),
        .gpr_2_r   (src2),
        .gpr_w_addr(rd),
        .gpr_1_addr(rs1),
        .gpr_2_addr(rs2),
        .gpr_w_en  (gpr_w_en)
    );

    wire [`ysyx_23060075_ISA_WIDTH-1:0] csr_w;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] csr_r;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] zimm_or_src1 = is_csri ? zimm : src1;

    ysyx_23060075_mux_def #(
        .NR_KEY  (3),
        .KEY_LEN (2),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_csr_w (
        .out(csr_w),
        .key(funct3[1:0]),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({2'b01, zimm_or_src1, 2'b10, csr_r | zimm_or_src1, 2'b11, csr_r & ~zimm_or_src1})
    );

    ysyx_23060075_csr csr_1 (
        .clk     (clk),
        .rst     (rst),
        .csr_w   (csr_w),
        .csr_r   (csr_r),
        .csr_addr(csr_addr),
        .csr_w_en(csr_w_en)
    );

endmodule
