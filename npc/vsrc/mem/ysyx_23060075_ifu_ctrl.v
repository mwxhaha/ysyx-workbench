`include "ysyx_23060075_isa.vh"

module ysyx_23060075_ifu_ctrl (
    input wire clk,
    input wire rst,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    input  wire                                mem_1_r_en,
    output wire                                mem_1_finish,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    output reg                                 axi_arvalid,
    input  wire                                axi_arready,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                axi_rvalid,
    output reg                                 axi_rready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    output reg                                 axi_awvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                axi_awready,
    // verilator lint_on UNUSEDSIGNAL

    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    output reg  [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    output reg                                      axi_wvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                     axi_wready,
    // verilator lint_on UNUSEDSIGNAL

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    input  wire                                axi_bvalid,
    // verilator lint_on UNUSEDSIGNAL
    output reg                                 axi_bready
);

    // read address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_araddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_1_r_en) axi_araddr <= mem_1_addr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_arvalid <= 1'b0;
        else if (axi_arvalid && axi_arready) axi_arvalid <= 1'b0;
        else if (mem_1_r_en) axi_arvalid <= 1'b1;
    end

    // read data channel
    always @(posedge clk, posedge rst) begin
        if (rst) mem_1_r <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_rvalid && axi_rready) mem_1_r <= axi_rdata;
    end
    ysyx_23060075_pluse pluse_mem_1_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_rvalid && axi_rready),
        .dout(mem_1_finish)
    );
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rready <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_rready <= 1'b0;
        else if (axi_arvalid && axi_arready) axi_rready <= 1'b1;
    end

    // write address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awaddr <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awvalid <= 1'b0;
    end

    // write data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wdata <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wstrb <= `ysyx_23060075_MEM_MASK_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wvalid <= 1'b0;
    end

    // write response channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bready <= 1'b0;
    end

endmodule
