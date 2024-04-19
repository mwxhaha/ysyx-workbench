`include "ysyx_23060075_isa.vh"

module ysyx_23060075_clint (
    input wire clk,
    input wire rst,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    input  wire                                axi_arvalid,
    output reg                                 axi_arready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    output reg                                 axi_rvalid,
    input  wire                                axi_rready,

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    input  wire                                axi_awvalid,
    // verilator lint_on UNUSEDSIGNAL
    output reg                                 axi_awready,

    // verilator lint_off UNUSEDSIGNAL
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    input  wire                                     axi_wvalid,
    // verilator lint_on UNUSEDSIGNAL
    output reg                                      axi_wready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    output reg                                 axi_bvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                axi_bready
    // verilator lint_on UNUSEDSIGNAL
);

    reg [63:0] cnt;
    always @(posedge clk, posedge rst) begin
        if (rst) cnt <= 64'd0;
        else cnt <= cnt + 64'd1;
    end

    // read address channel
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr_reg;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_araddr_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_arvalid && axi_arready) axi_araddr_reg <= axi_araddr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_arready <= 1'b1;
        else if (axi_arvalid && axi_arready) axi_arready <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_arready <= 1'b1;
    end

    // read data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_arvalid && axi_arready) begin
            if (axi_araddr_reg == `ysyx_23060075_ISA_WIDTH'ha0000048) axi_rdata <= cnt[31:0];
            else if (axi_araddr_reg == `ysyx_23060075_ISA_WIDTH'ha000004c) axi_rdata <= cnt[63:32];
        end
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rvalid <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_rvalid <= 1'b0;
        else if (axi_arvalid && axi_arready) axi_rvalid <= 1'b1;
    end

    // write address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awready <= 1'b0;
    end

    // write data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wready <= 1'b0;
    end

    // write response channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bvalid <= 1'b0;
    end

endmodule
