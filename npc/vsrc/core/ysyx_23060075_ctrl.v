`include "config.vh"

module ysyx_23060075_ctrl (
    input  wire [`ysyx_23060075_INST_TYPE_WIDTH-1:0] inst_type,
    input  wire [   `ysyx_23060075_OPCODE_WIDTH-1:0] opcode,
    input  wire [   `ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    // verilator lint_off UNUSEDSIGNAL
    input  wire [   `ysyx_23060075_FUNCT7_WIDTH-1:0] funct7,
    // verilator lint_on UNUSEDSIGNAL
    output wire                                      pc_en,
    output wire                                      is_branch,
    output wire                                      is_jal,
    output wire                                      is_jalr,
    output wire                                      mem_if_en,
    output wire                                      gpr_w_en,
    output wire                                      alu_b_is_imm,
    output wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct,
    output wire                                      mem_r_en,
    output wire                                      mem_w_en,
    output wire [ `ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_mask,
    output wire                                      rd_is_mem,
    output wire                                      is_lui,
    output wire                                      is_auipc,
    output wire                                      is_csri,
    output wire                                      csr_w_en
);

    assign pc_en = 1'b1;
    assign is_branch = opcode == `ysyx_23060075_OPCODE_WIDTH'b1100011;
    assign is_jal = opcode == `ysyx_23060075_OPCODE_WIDTH'b1101111;
    assign is_jalr = opcode == `ysyx_23060075_OPCODE_WIDTH'b1100111;
    assign mem_if_en = 1'b1;
    assign gpr_w_en = inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_R | inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I | inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_U | inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_J;
    assign alu_b_is_imm = inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_I | inst_type == `ysyx_23060075_INST_TYPE_WIDTH'd`ysyx_23060075_S;

    wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct_beq;
    wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct_addi = funct3 == `ysyx_23060075_FUNCT3_WIDTH'b101 ? {funct7[5],funct3} : ( funct3 == `ysyx_23060075_FUNCT3_WIDTH'b010 | funct3 == `ysyx_23060075_FUNCT3_WIDTH'b011 ? {1'b1,funct3} : {1'b0,funct3});
    wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct_add = funct3 == `ysyx_23060075_FUNCT3_WIDTH'b010 | funct3 == `ysyx_23060075_FUNCT3_WIDTH'b011 ? {1'b1,funct3} : {funct7[5],funct3};

    ysyx_23060075_mux_def #(
        .NR_KEY  (6),
        .KEY_LEN (`ysyx_23060075_FUNCT3_WIDTH),
        .DATA_LEN(`ysyx_23060075_ALU_FUNCT_WIDTH)
    ) mux_def_alu_funct_beq (
        .out(alu_funct_beq),
        .key(funct3),
        .default_out(`ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT),
        .lut({
            `ysyx_23060075_FUNCT3_WIDTH'b000,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_EQ,
            `ysyx_23060075_FUNCT3_WIDTH'b001,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NE,
            `ysyx_23060075_FUNCT3_WIDTH'b100,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_LT,
            `ysyx_23060075_FUNCT3_WIDTH'b101,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_GE,
            `ysyx_23060075_FUNCT3_WIDTH'b110,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_LTU,
            `ysyx_23060075_FUNCT3_WIDTH'b111,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_GEU
        })
    );

    ysyx_23060075_mux_def #(
        .NR_KEY  (`ysyx_23060075_OPCODE_NUMBER_MAX),
        .KEY_LEN (`ysyx_23060075_OPCODE_WIDTH),
        .DATA_LEN(`ysyx_23060075_ALU_FUNCT_WIDTH)
    ) mux_def_alu_funct (
        .out(alu_funct),
        .key(opcode),
        .default_out(`ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT),
        .lut({
            `ysyx_23060075_OPCODE_WIDTH'b0110111,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT,
            `ysyx_23060075_OPCODE_WIDTH'b0010111,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT,
            `ysyx_23060075_OPCODE_WIDTH'b1101111,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT,
            `ysyx_23060075_OPCODE_WIDTH'b1100111,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_ADD,
            `ysyx_23060075_OPCODE_WIDTH'b1100011,
            alu_funct_beq,
            `ysyx_23060075_OPCODE_WIDTH'b0000011,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_ADD,
            `ysyx_23060075_OPCODE_WIDTH'b0100011,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_ADD,
            `ysyx_23060075_OPCODE_WIDTH'b0010011,
            alu_funct_addi,
            `ysyx_23060075_OPCODE_WIDTH'b0110011,
            alu_funct_add,
            `ysyx_23060075_OPCODE_WIDTH'b1110011,
            `ysyx_23060075_ALU_FUNCT_WIDTH'd`ysyx_23060075_NO_FUNCT
        })
    );

    assign mem_r_en = opcode == `ysyx_23060075_OPCODE_WIDTH'b0000011;
    assign mem_w_en = opcode == `ysyx_23060075_OPCODE_WIDTH'b0100011;
    assign mem_mask = funct3 == `ysyx_23060075_FUNCT3_WIDTH'b000 ? `ysyx_23060075_MEM_MASK_WIDTH'b0001 : (funct3 == `ysyx_23060075_FUNCT3_WIDTH'b001 ? `ysyx_23060075_MEM_MASK_WIDTH'b0011 : `ysyx_23060075_MEM_MASK_WIDTH'b1111);
    assign rd_is_mem = opcode == `ysyx_23060075_OPCODE_WIDTH'b0000011;
    assign is_lui = opcode == `ysyx_23060075_OPCODE_WIDTH'b0110111;
    assign is_auipc = opcode == `ysyx_23060075_OPCODE_WIDTH'b0010111;
    assign is_csri = opcode == `ysyx_23060075_OPCODE_WIDTH'b1110011 & funct3[2]==1'b1;
    assign csr_w_en = opcode == `ysyx_23060075_OPCODE_WIDTH'b1110011 & funct3[1:0]!=2'b00;

endmodule
