`include "config.vh"

module ctrl (
    input  wire [`INST_TYPE_WIDTH-1:0] inst_type,
    input  wire [   `OPCODE_WIDTH-1:0] opcode,
    input  wire [   `FUNCT3_WIDTH-1:0] funct3,
// verilator lint_off UNUSEDSIGNAL
    input  wire [   `FUNCT7_WIDTH-1:0] funct7,
// verilator lint_on UNUSEDSIGNAL
    output wire                        pc_en,
    output wire                        is_branch,
    output wire                        is_jal,
    output wire                        is_jalr,
    output wire                        mem_if_en,
    output wire                        gpr_w_en,
    output wire                        alu_b_is_imm,
    output wire [`ALU_FUNCT_WIDTH-1:0] alu_funct,
    output wire                        mem_r_en,
    output wire                        mem_w_en,
    output wire [ `MEM_MASK_WIDTH-1:0] mem_mask,
    output wire                        rd_is_mem,
    output wire                        is_lui,
    output wire                        is_auipc
);

    assign pc_en = 1'b1;
    assign is_branch = opcode == `OPCODE_WIDTH'b1100011;
    assign is_jal = opcode == `OPCODE_WIDTH'b1101111;
    assign is_jalr = opcode == `OPCODE_WIDTH'b1100111;
    assign mem_if_en = 1'b1;
    assign gpr_w_en = inst_type == `INST_TYPE_WIDTH'd`R | inst_type == `INST_TYPE_WIDTH'd`I | inst_type == `INST_TYPE_WIDTH'd`U | inst_type == `INST_TYPE_WIDTH'd`J;
    assign alu_b_is_imm = inst_type == `INST_TYPE_WIDTH'd`I | inst_type == `INST_TYPE_WIDTH'd`S;

    wire [`ALU_FUNCT_WIDTH-1:0] alu_funct_beq;
    wire [`ALU_FUNCT_WIDTH-1:0] alu_funct_addi = funct3 == `FUNCT3_WIDTH'b101 ? {funct7[5],funct3} : ( funct3 == `FUNCT3_WIDTH'b010 | funct3 == `FUNCT3_WIDTH'b011 ? {1'b1,funct3} : {1'b0,funct3});
    wire [`ALU_FUNCT_WIDTH-1:0] alu_funct_add = funct3 == `FUNCT3_WIDTH'b010 | funct3 == `FUNCT3_WIDTH'b011 ? {1'b1,funct3} : {funct7[5],funct3};

    MuxKeyWithDefault #(
        .NR_KEY  (6),
        .KEY_LEN (`FUNCT3_WIDTH),
        .DATA_LEN(`ALU_FUNCT_WIDTH)
    ) muxkeywithdefault_alu_funct_beq (
        .out(alu_funct_beq),
        .key(funct3),
        .default_out(`ALU_FUNCT_WIDTH'd`NO_FUNCT),
        .lut({
            `FUNCT3_WIDTH'b000,
            `ALU_FUNCT_WIDTH'd`EQ,
            `FUNCT3_WIDTH'b001,
            `ALU_FUNCT_WIDTH'd`NE,
            `FUNCT3_WIDTH'b100,
            `ALU_FUNCT_WIDTH'd`LT,
            `FUNCT3_WIDTH'b101,
            `ALU_FUNCT_WIDTH'd`GE,
            `FUNCT3_WIDTH'b110,
            `ALU_FUNCT_WIDTH'd`LTU,
            `FUNCT3_WIDTH'b111,
            `ALU_FUNCT_WIDTH'd`GEU
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`OPCODE_NUMBER_MAX),
        .KEY_LEN (`OPCODE_WIDTH),
        .DATA_LEN(`ALU_FUNCT_WIDTH)
    ) muxkeywithdefault_alu_funct (
        .out(alu_funct),
        .key(opcode),
        .default_out(`ALU_FUNCT_WIDTH'd`NO_FUNCT),
        .lut({
            `OPCODE_WIDTH'b0110111,
            `ALU_FUNCT_WIDTH'd`NO_FUNCT,
            `OPCODE_WIDTH'b0010111,
            `ALU_FUNCT_WIDTH'd`NO_FUNCT,
            `OPCODE_WIDTH'b1101111,
            `ALU_FUNCT_WIDTH'd`NO_FUNCT,
            `OPCODE_WIDTH'b1100111,
            `ALU_FUNCT_WIDTH'd`ADD,
            `OPCODE_WIDTH'b1100011,
            alu_funct_beq,
            `OPCODE_WIDTH'b0000011,
            `ALU_FUNCT_WIDTH'd`ADD,
            `OPCODE_WIDTH'b0100011,
            `ALU_FUNCT_WIDTH'd`ADD,
            `OPCODE_WIDTH'b0010011,
            alu_funct_addi,
            `OPCODE_WIDTH'b0110011,
            alu_funct_add,
            `OPCODE_WIDTH'b1110011,
            `ALU_FUNCT_WIDTH'd`NO_FUNCT
        })
    );

    assign mem_r_en = opcode == `OPCODE_WIDTH'b0000011;
    assign mem_w_en = opcode == `OPCODE_WIDTH'b0100011;
    assign mem_mask = funct3 == `FUNCT3_WIDTH'b000 ? `MEM_MASK_WIDTH'b0001 : (funct3 == `FUNCT3_WIDTH'b001 ? `MEM_MASK_WIDTH'b0011 : `MEM_MASK_WIDTH'b1111);
    assign rd_is_mem = opcode == `OPCODE_WIDTH'b0000011;
    assign is_lui = opcode == `OPCODE_WIDTH'b0110111;
    assign is_auipc = opcode == `OPCODE_WIDTH'b0010111;

endmodule
