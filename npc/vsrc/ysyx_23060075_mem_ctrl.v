`include "config.vh"

module ysyx_23060075_mem_ctrl (
    input wire clk,
    input wire rst,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    input  wire                                mem_1_r_en,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    input  wire                                     mem_2_r_en,
    input  wire                                     mem_2_w_en
);

    always @(*) begin
        if (!rst && mem_1_r_en) begin
            addr_ifetch_dpic(mem_1_addr, mem_1_r);
        end else begin
            mem_1_r = `ysyx_23060075_ISA_WIDTH'b0;
        end
    end

    reg [`ysyx_23060075_ISA_WIDTH-1:0] pmem_2_r;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] pmem_2_w;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] pmem_2_addr = {
        mem_2_addr[`ysyx_23060075_ISA_WIDTH-1:2], 2'b0
    };
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] pmem_2_mask;
    wire pmem_2_r_en = mem_2_r_en;
    wire pmem_2_w_en = mem_2_w_en;
    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_mem_2_r (
        .out(mem_2_r),
        .key(mem_2_addr[1:0]),
        .lut({
            2'b00,
            pmem_2_r,
            2'b01,
            {8'b0, pmem_2_r[`ysyx_23060075_ISA_WIDTH-1:8]},
            2'b10,
            {16'b0, pmem_2_r[`ysyx_23060075_ISA_WIDTH-1:16]},
            2'b11,
            {24'b0, pmem_2_r[`ysyx_23060075_ISA_WIDTH-1:24]}
        })
    );
    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_mem_2_w (
        .out(pmem_2_w),
        .key(mem_2_addr[1:0]),
        .lut({
            2'b00,
            mem_2_w,
            2'b01,
            {mem_2_w[`ysyx_23060075_ISA_WIDTH-9:0], 8'b0},
            2'b10,
            {mem_2_w[`ysyx_23060075_ISA_WIDTH-17:0], 16'b0},
            2'b11,
            {mem_2_w[`ysyx_23060075_ISA_WIDTH-25:0], 24'b0}
        })
    );
    ysyx_23060075_mux #(
        .NR_KEY  (4),
        .KEY_LEN (2),
        .DATA_LEN(`ysyx_23060075_MEM_MASK_WIDTH)
    ) mux_mem_2_mask (
        .out(pmem_2_mask),
        .key(mem_2_addr[1:0]),
        .lut({
            2'b00,
            mem_2_mask,
            2'b01,
            {mem_2_mask[2:0], 1'b0},
            2'b10,
            {mem_2_mask[1:0], 2'b0},
            2'b11,
            {mem_2_mask[0], 3'b0}
        })
    );

    always @(negedge clk) begin
        if (!rst && pmem_2_r_en) begin
            addr_read_dpic(pmem_2_addr, pmem_2_r);
        end
    end
    always @(posedge clk) begin
        if (!rst && pmem_2_w_en) begin
            addr_write_dpic(pmem_2_addr, {4'b0000, pmem_2_mask}, pmem_2_w);
        end
    end

endmodule
