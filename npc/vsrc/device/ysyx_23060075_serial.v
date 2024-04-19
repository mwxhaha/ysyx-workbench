`include "ysyx_23060075_isa.vh"

module ysyx_23060075_serial (
    input wire clk,
    input wire rst,

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    input  wire                                axi_arvalid,
    // verilator lint_on UNUSEDSIGNAL
    output reg                                 axi_arready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    output reg                                 axi_rvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                axi_rready,
    // verilator lint_on UNUSEDSIGNAL

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    input  wire                                axi_awvalid,
    output reg                                 axi_awready,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                     axi_wvalid,
    output reg                                      axi_wready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    output reg                                 axi_bvalid,
    input  wire                                axi_bready
);

    // read address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_arready <= 1'b0;
    end

    // read data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rdata <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rvalid <= 1'b0;
    end

    // write address channel
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr_reg;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awaddr_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_awvalid && axi_awready) axi_awaddr_reg <= axi_awaddr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awready <= 1'b1;
        else if (axi_awvalid && axi_awready) axi_awready <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_awready <= 1'b1;
    end

    // write data channel
    // verilator lint_off UNUSEDSIGNAL
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_wdata_reg;
    // verilator lint_on UNUSEDSIGNAL
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wdata_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_wvalid && axi_wready) axi_wdata_reg <= axi_wdata;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wready <= 1'b1;
        else if (axi_wvalid && axi_wready) axi_wready <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_wready <= 1'b1;
    end

    // write response channel
    reg is_output;
    always @(posedge clk, posedge rst) begin
        if (rst) is_output <= 1'b0;
        else if (axi_wvalid && axi_wready) is_output <= 1'b1;
        else is_output <= 1'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst);
        else if (is_output && axi_awaddr_reg == `ysyx_23060075_ISA_WIDTH'ha00003f8)
            $write("%c", axi_wdata_reg[7:0]);
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bvalid <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_bvalid <= 1'b0;
        else if (axi_wvalid && axi_wready) axi_bvalid <= 1'b1;
    end

endmodule
