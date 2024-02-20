`include "config.vh"

module ysyx_23060075_wbu (
    input  wire [   `ysyx_23060075_ISA_WIDTH-1:0] mem_r,
    input  wire [   `ysyx_23060075_ISA_WIDTH-1:0] alu_result,
    input  wire [`ysyx_23060075_FUNCT3_WIDTH-1:0] funct3,
    input  wire [   `ysyx_23060075_IMM_WIDTH-1:0] imm,
    input  wire [   `ysyx_23060075_ISA_WIDTH-1:0] snpc,
    input  wire [   `ysyx_23060075_ISA_WIDTH-1:0] pc_imm,
    output wire [   `ysyx_23060075_ISA_WIDTH-1:0] srd,
    input  wire                                   rd_is_mem,
    input  wire                                   is_lui,
    input  wire                                   is_auipc,
    input  wire                                   is_jal,
    input  wire                                   is_jalr
);

    wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_r_shift;
    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_mem_r_shift (
        .out(mem_r_shift),
        .key(alu_result[1:0]),
        .lut({
            2'b00,
            mem_r,
            2'b01,
            {8'b0, mem_r[`ysyx_23060075_ISA_WIDTH-1:8]},
            2'b10,
            {16'b0, mem_r[`ysyx_23060075_ISA_WIDTH-1:16]},
            2'b11,
            {24'b0, mem_r[`ysyx_23060075_ISA_WIDTH-1:24]}
        })
    );

    wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_r_extend;
    ysyx_23060075_mux_def #(
        .NR_KEY  (5),
        .KEY_LEN (`ysyx_23060075_FUNCT3_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_mem_r_extend (
        .out(mem_r_extend),
        .key(funct3),
        .default_out(`ysyx_23060075_ISA_WIDTH'b0),
        .lut({
            `ysyx_23060075_FUNCT3_WIDTH'b000,
            {{24{mem_r_shift[7]}}, mem_r_shift[7:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b001,
            {{16{mem_r_shift[15]}}, mem_r_shift[15:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b010,
            mem_r_shift,
            `ysyx_23060075_FUNCT3_WIDTH'b100,
            {24'b0, mem_r_shift[7:0]},
            `ysyx_23060075_FUNCT3_WIDTH'b101,
            {16'b0, mem_r_shift[15:0]}
        })
    );

    assign srd = is_lui ? imm : (is_auipc ? pc_imm : (is_jal | is_jalr ? snpc : (rd_is_mem ? mem_r_extend : alu_result)));

endmodule
