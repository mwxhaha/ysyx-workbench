`include "ysyx_23060075_isa.vh"

module ysyx_23060075_lsu_core (
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] src2,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_r,
    input  wire [  `ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_mask,
    input  wire                                     mem_r_en,
    input  wire                                     mem_w_en,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                                     mem_2_r_en,
    output wire                                     mem_2_w_en
);

    ysyx_23060075_mux_def #(
        .NR_KEY  (5),
        .KEY_LEN (`ysyx_23060075_FUNCT3_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_mem_r (
        .out(mem_r),
        .key(funct3),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_FUNCT3_WIDTH'b000,
            {{24{mem_2_r[7]}}, mem_2_r[7:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b001,
            {{16{mem_2_r[15]}}, mem_2_r[15:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b010,
            mem_2_r,
            `ysyx_23060075_FUNCT3_WIDTH'b100,
            {24'b0, mem_2_r[7:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b101,
            {16'b0, mem_2_r[15:0]}
        })
    );
    ysyx_23060075_mux_def #(
        .NR_KEY  (3),
        .KEY_LEN (`ysyx_23060075_FUNCT3_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_mem_2_w (
        .out(mem_2_w),
        .key(funct3),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_FUNCT3_WIDTH'b000,
            {24'b0, src2[7:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b001,
            {16'b0, src2[15:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b010,
            src2
        })
    );
    assign mem_2_addr = alu_result;
    assign mem_2_mask = mem_mask;
    assign mem_2_r_en = mem_r_en;
    assign mem_2_w_en = mem_w_en;

endmodule
