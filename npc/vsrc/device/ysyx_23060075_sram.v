`include "ysyx_23060075_isa.vh"

module ysyx_23060075_sram (
    input wire clk,
    input wire rst,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    input  wire                                axi_arvalid,
    output reg                                 axi_arready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    output reg                                 axi_rvalid,
    input  wire                                axi_rready,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    input  wire                                axi_awvalid,
    output reg                                 axi_awready,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    input  wire                                     axi_wvalid,
    output reg                                      axi_wready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    output reg                                 axi_bvalid,
    input  wire                                axi_bready
);

`ifdef SYNTHESIS
    ysyx_23060075_reg_file #(
        .ADDR_WIDTH(8),
        .DATA_WIDTH(`ysyx_23060075_ISA_WIDTH)
    ) mem_1 (
        .clk    (clk),
        .rst    (rst),
        .wdata  (axi_wdata),
        .waddr  (axi_awaddr),
        .rdata_1(axI_rdata),
        .raddr_1(axi_araddr),
        .rdata_2(),
        .raddr_2(`ysyx_23060075_ISA_WIDTH'b0),
        .wen    (axi_wvalid && axi_wready)
    );
`else

    // read address channel
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr_reg;
    reg [`ysyx_23060075_ISA_WIDTH-1:0] rcnt;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_araddr_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_arvalid && axi_arready) axi_araddr_reg <= axi_araddr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) rcnt <= `ysyx_23060075_ISA_WIDTH'd0;
        else if (axi_arvalid && axi_arready) rcnt <= {$random} % 5 + `ysyx_23060075_ISA_WIDTH'd1;
        else if (rcnt != `ysyx_23060075_ISA_WIDTH'd0) rcnt <= rcnt - `ysyx_23060075_ISA_WIDTH'd1;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_arready <= 1'b1;
        else if (axi_arvalid && axi_arready) axi_arready <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_arready <= 1'b1;
    end

    // read data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (rcnt == `ysyx_23060075_ISA_WIDTH'd1) axi_rdata <= addr_read_dpic(axi_araddr_reg);
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rvalid <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_rvalid <= 1'b0;
        else if (rcnt == `ysyx_23060075_ISA_WIDTH'd1) axi_rvalid <= 1'b1;
    end

    // write address channel
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr_reg;
    reg [`ysyx_23060075_ISA_WIDTH-1:0] wcnt;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awaddr_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_awvalid && axi_awready) axi_awaddr_reg <= axi_awaddr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) wcnt <= `ysyx_23060075_ISA_WIDTH'd0;
        else if (axi_wvalid && axi_wready) wcnt <= {$random} % 5 + `ysyx_23060075_ISA_WIDTH'd1;
        else if (wcnt != `ysyx_23060075_ISA_WIDTH'd0) wcnt <= wcnt - `ysyx_23060075_ISA_WIDTH'd1;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awready <= 1'b1;
        else if (axi_awvalid && axi_awready) axi_awready <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_awready <= 1'b1;
    end

    // write data channel
    reg [`ysyx_23060075_ISA_WIDTH-1:0] axi_wdata_reg;
    // verilator lint_off UNUSEDSIGNAL
    reg [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb_reg;
    // verilator lint_on UNUSEDSIGNAL
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wdata_reg <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_wvalid && axi_wready) axi_wdata_reg <= axi_wdata;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wstrb_reg <= `ysyx_23060075_MEM_MASK_WIDTH'b0;
        else if (axi_wvalid && axi_wready) axi_wstrb_reg <= axi_wstrb;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wready <= 1'b1;
        else if (axi_wvalid && axi_wready) axi_wready <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_wready <= 1'b1;
    end

    // write response channel
    always @(posedge clk, posedge rst) begin
        if (rst);
        else if (wcnt == `ysyx_23060075_ISA_WIDTH'd1)
            addr_write_dpic(axi_awaddr_reg, {
                            {8 - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}}, axi_wstrb_reg},
                            axi_wdata_reg);
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bresp <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bvalid <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_bvalid <= 1'b0;
        else if (wcnt == `ysyx_23060075_ISA_WIDTH'd1) axi_bvalid <= 1'b1;
    end
`endif

endmodule
