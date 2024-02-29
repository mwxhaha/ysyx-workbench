`include "config.vh"

module ysyx_23060075_exu (
    input  wire [      `ysyx_23060075_IMM_WIDTH-1:0] imm,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] src1,
    input  wire [      `ysyx_23060075_ISA_WIDTH-1:0] src2,
    output wire [      `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input  wire                                      alu_b_is_imm,
    input  wire [`ysyx_23060075_ALU_FUNCT_WIDTH-1:0] alu_funct,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] pc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pc_imm
);

    wire [`ysyx_23060075_ISA_WIDTH-1:0] alu_b = alu_b_is_imm ? imm : src2;
    ysyx_23060075_alu alu_exu (
        .alu_a     (src1),
        .alu_b     (alu_b),
        .alu_funct (alu_funct),
        .alu_result(alu_result)
    );

    ysyx_23060075_adder #(
        .data_len(`ysyx_23060075_ISA_WIDTH)
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
