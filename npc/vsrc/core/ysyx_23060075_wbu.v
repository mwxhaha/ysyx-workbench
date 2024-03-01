`include "config.vh"

module ysyx_23060075_wbu (
    input  wire [        `ysyx_23060075_IMM_WIDTH-1:0] imm,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] pc_imm,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] snpc,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] mem_r,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input  wire [        `ysyx_23060075_ISA_WIDTH-1:0] csr_r,
    output wire [        `ysyx_23060075_ISA_WIDTH-1:0] srd,
    input  wire [`ysyx_23060075_SRD_MUX_SEL_WIDTH-1:0] srd_mux_sel
);

    ysyx_23060075_mux_def #(
        .NR_KEY  (6),
        .KEY_LEN (`ysyx_23060075_SRD_MUX_SEL_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_srd (
        .out(srd),
        .key(srd_mux_sel),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_SRD_IS_IMM,
            imm,
            `ysyx_23060075_SRD_IS_PC_IMM,
            pc_imm,
            `ysyx_23060075_SRD_IS_SNPC,
            snpc,
            `ysyx_23060075_SRD_IS_MEM_R,
            mem_r,
            `ysyx_23060075_SRD_IS_ALU_RESULT,
            alu_result,
            `ysyx_23060075_SRD_IS_CSR_R,
            csr_r
        })
    );

endmodule
