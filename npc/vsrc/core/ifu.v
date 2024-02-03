`include "config.vh"

module ifu (
    input  wire                  clk,
    input  wire                  rst,
    input  wire [`ISA_WIDTH-1:0] pc_imm,
    input  wire [`ISA_WIDTH-1:0] alu_result,
    input  wire                  is_branch,
    input  wire                  is_jal,
    input  wire                  is_jalr,
    output wire [`ISA_WIDTH-1:0] pc,
    output wire [`ISA_WIDTH-1:0] snpc,
    input  wire                  pc_en,
    input  wire [`ISA_WIDTH-1:0] mem_1_r,
    output wire [`ISA_WIDTH-1:0] mem_1_addr,
    output wire                  mem_1_r_en,
    input  wire                  mem_if_en,
    output wire [`ISA_WIDTH-1:0] inst
);

    wire [`ISA_WIDTH-1:0] dnpc = is_jalr ? (alu_result & ~`ISA_WIDTH'b1) : (alu_result[0] & is_branch | is_jal ? pc_imm : snpc);

    pc pc_1 (
        .clk   (clk),
        .rst   (rst),
        .pc_in (dnpc),
        .pc_out(pc),
        .pc_en (pc_en)
    );

    adder #(
        .data_len(`ISA_WIDTH)
    ) adder_snpc (
        .a   (pc),
        .b   (`ISA_WIDTH'd4),
        .cin (1'b0),
        .s   (snpc),
// verilator lint_off PINCONNECTEMPTY
        .cout()
// verilator lint_on PINCONNECTEMPTY
    );

    assign inst = mem_1_r;
    assign mem_1_addr = pc;
    assign mem_1_r_en = mem_if_en;

endmodule
