`include "ysyx_23060075_isa.vh"

module ysyx_23060075_ifu_core (
    input wire clk,
    input wire rst,

    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] pc_imm,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] mtvec,
    input wire [         `ysyx_23060075_ISA_WIDTH-1:0] mepc,
    input wire [`ysyx_23060075_DNPC_MUX_SEL_WIDTH-1:0] dnpc_mux_sel,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pc,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] snpc,
    input  wire                                pc_en,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] inst,
    input  wire                                mem_if_en,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    output wire                                mem_1_r_en
);

    wire [`ysyx_23060075_ISA_WIDTH-1:0] dnpc;
    ysyx_23060075_mux_def #(
        .NR_KEY  (6),
        .KEY_LEN (`ysyx_23060075_DNPC_MUX_SEL_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_dnpc (
        .out(dnpc),
        .key(dnpc_mux_sel),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_DNPC_IS_SNPC,
            snpc,
            `ysyx_23060075_DNPC_IS_PC_IMM,
            pc_imm,
            `ysyx_23060075_DNPC_IS_ALU_RESULT,
            alu_result & ~`ysyx_23060075_ISA_WIDTH'b1,
            `ysyx_23060075_DNPC_IS_BRANCH,
            alu_result[0] ? pc_imm : snpc,
            `ysyx_23060075_DNPC_IS_MTVEC,
            mtvec,
            `ysyx_23060075_DNPC_IS_MEPC,
            mepc
        })
    );

    ysyx_23060075_pc pc_1 (
        .clk   (clk),
        .rst   (rst),
        .pc_in (dnpc),
        .pc_out(pc),
        .pc_en (pc_en)
    );
    ysyx_23060075_adder #(
        .data_len(`ysyx_23060075_ISA_WIDTH)
    ) adder_snpc (
        .a   (pc),
        .b   (`ysyx_23060075_ISA_WIDTH'd4),
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
