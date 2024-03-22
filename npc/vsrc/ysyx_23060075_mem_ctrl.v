`include "ysyx_23060075_isa.vh"

module ysyx_23060075_mem_ctrl (
    // verilator lint_off UNUSEDSIGNAL
    input wire clk,
    input wire rst,
    // verilator lint_on UNUSEDSIGNAL

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    input  wire                                mem_1_r_en,
    output wire                                mem_1_finish,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] raddr_1,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] rdata_1,
    output wire                                rvalid_1,
    input  wire                                rready_1,

    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    input  wire                                     mem_2_r_en,
    input  wire                                     mem_2_w_en,
    output wire                                     mem_2_finish,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] raddr_2,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] rdata_2,
    output wire                                     rvalid_2,
    input  wire                                     rready_2,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] waddr_2,
    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] wdata_2,
    output reg  [`ysyx_23060075_MEM_MASK_WIDTH-1:0] wmask_2,
    output wire                                     wvalid_2,
    input  wire                                     wready_2
);

    assign raddr_1 = mem_1_addr;
    assign mem_1_r = rdata_1;
    assign rvalid_1 = mem_1_r_en;
    assign mem_1_finish = rready_1;

    assign raddr_2 = {mem_2_addr[`ysyx_23060075_ISA_WIDTH-1:2], 2'b0};
    always @(*) begin
        case (mem_2_addr[1:0])
            2'b00: mem_2_r = rdata_2;
            2'b01: mem_2_r = {8'b0, rdata_2[`ysyx_23060075_ISA_WIDTH-1:8]};
            2'b10: mem_2_r = {16'b0, rdata_2[`ysyx_23060075_ISA_WIDTH-1:16]};
            2'b11: mem_2_r = {24'b0, rdata_2[`ysyx_23060075_ISA_WIDTH-1:24]};
        endcase
    end
    assign waddr_2 = {mem_2_addr[`ysyx_23060075_ISA_WIDTH-1:2], 2'b0};
    always @(*) begin
        case (mem_2_addr[1:0])
            2'b00: wdata_2 = mem_2_w;
            2'b01: wdata_2 = {mem_2_w[`ysyx_23060075_ISA_WIDTH-9:0], 8'b0};
            2'b10: wdata_2 = {mem_2_w[`ysyx_23060075_ISA_WIDTH-17:0], 16'b0};
            2'b11: wdata_2 = {mem_2_w[`ysyx_23060075_ISA_WIDTH-25:0], 24'b0};
        endcase
    end
    always @(*) begin
        case (mem_2_addr[1:0])
            2'b00: wmask_2 = mem_2_mask;
            2'b01: wmask_2 = {mem_2_mask[2:0], 1'b0};
            2'b10: wmask_2 = {mem_2_mask[1:0], 2'b0};
            2'b11: wmask_2 = {mem_2_mask[0], 3'b0};
        endcase
    end
    assign rvalid_2     = mem_2_r_en;
    assign wvalid_2     = mem_2_w_en;
    assign mem_2_finish = rready_2 | wready_2;

endmodule
