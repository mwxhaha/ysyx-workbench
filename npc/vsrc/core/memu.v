`include "config.vh"

module memu (
    input  wire [  `FUNCT3_WIDTH-1:0] funct3,
    input  wire [     `ISA_WIDTH-1:0] alu_result,
    input  wire [     `ISA_WIDTH-1:0] src2,
    output wire [     `ISA_WIDTH-1:0] mem_r,
    input  wire [`MEM_MASK_WIDTH-1:0] mem_mask,
    input  wire                       mem_r_en,
    input  wire                       mem_w_en,
    input  wire [     `ISA_WIDTH-1:0] mem_2_r,
    output wire [     `ISA_WIDTH-1:0] mem_2_w,
    output wire [     `ISA_WIDTH-1:0] mem_2_addr,
    output wire [`MEM_MASK_WIDTH-1:0] mem_2_mask,
    output wire                       mem_2_r_en,
    output wire                       mem_2_w_en
);

    assign mem_r = mem_2_r;

    wire [`ISA_WIDTH-1:0] mem_w_extend;
    MuxKeyWithDefault #(
        .NR_KEY  (3),
        .KEY_LEN (`FUNCT3_WIDTH),
        .DATA_LEN(`ISA_WIDTH)
    ) muxkeywithdefault_mem_w_extend (
        .out(mem_w_extend),
        .key(funct3),
        .default_out(`ISA_WIDTH'b0),
        .lut({
            `FUNCT3_WIDTH'b000,
            {24'b0, src2[7:0]},
            `FUNCT3_WIDTH'b001,
            {16'b0, src2[15:0]},
            `FUNCT3_WIDTH'b010,
            src2
        })
    );
    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`ISA_WIDTH)
    ) muxkey_mem_2_w (
        .out(mem_2_w),
        .key(alu_result[1:0]),
        .lut({
            2'b00,
            mem_w_extend,
            2'b01,
            {mem_w_extend[`ISA_WIDTH-9:0], 8'b0},
            2'b10,
            {mem_w_extend[`ISA_WIDTH-17:0], 16'b0},
            2'b11,
            {mem_w_extend[`ISA_WIDTH-25:0], 24'b0}
        })
    );

    assign mem_2_addr = {alu_result[`ISA_WIDTH-1:2], 2'b0};

    MuxKey #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`MEM_MASK_WIDTH)
    ) muxkey_mem_2_mask (
        .out(mem_2_mask),
        .key(alu_result[1:0]),
        .lut({
            2'b00,
            mem_mask,
            2'b01,
            {mem_mask[2:0], 1'b0},
            2'b10,
            {mem_mask[1:0], 2'b0},
            2'b11,
            {mem_mask[0], 3'b0}
        })
    );

    assign mem_2_r_en = mem_r_en;
    assign mem_2_w_en = mem_w_en;

endmodule
