`include "config.vh"

module exu (
    input  wire [      `IMM_WIDTH-1:0] imm,
    input  wire [      `ISA_WIDTH-1:0] src1,
    input  wire [      `ISA_WIDTH-1:0] src2,
    input  wire                        alu_b_is_imm,
    input  wire [`ALU_FUNCT_WIDTH-1:0] alu_funct,
    output wire [      `ISA_WIDTH-1:0] alu_result,
    input  wire [      `ISA_WIDTH-1:0] pc,
    output wire [      `ISA_WIDTH-1:0] pc_imm
);

    wire [`ISA_WIDTH-1:0] alu_b = alu_b_is_imm ? imm : src2;

    alu alu_exu (
        .alu_a     (src1),
        .alu_b     (alu_b),
        .alu_funct (alu_funct),
        .alu_result(alu_result)
    );

    adder #(
        .data_len(`ISA_WIDTH)
    ) adder_pc_imm (
        .a   (pc),
        .b   (imm),
        .cin (1'b0),
        .s   (pc_imm),
// verilator lint_off PINCONNECTEMPTY
        .cout()
// verilator lint_on PINCONNECTEMPTY
    );

endmodule
