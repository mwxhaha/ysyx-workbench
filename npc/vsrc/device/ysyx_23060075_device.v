`include "ysyx_23060075_isa.vh"

module ysyx_23060075_device (
    input wire clk,
    input wire rst,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    input  wire                                     axi_arvalid,
    output wire                                     axi_arready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    output wire                                     axi_rvalid,
    input  wire                                     axi_rready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    input  wire                                     axi_awvalid,
    output wire                                     axi_awready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    input  wire                                     axi_wvalid,
    output wire                                     axi_wready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    output wire                                     axi_bvalid,
    input  wire                                     axi_bready
);

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr;
    wire                                     axi_1_arvalid;
    wire                                     axi_1_arready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp;
    wire                                     axi_1_rvalid;
    wire                                     axi_1_rready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr;
    wire                                     axi_1_awvalid;
    wire                                     axi_1_awready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_1_wstrb;
    wire                                     axi_1_wvalid;
    wire                                     axi_1_wready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp;
    wire                                     axi_1_bvalid;
    wire                                     axi_1_bready;

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr;
    wire                                     axi_2_arvalid;
    wire                                     axi_2_arready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp;
    wire                                     axi_2_rvalid;
    wire                                     axi_2_rready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr;
    wire                                     axi_2_awvalid;
    wire                                     axi_2_awready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_2_wstrb;
    wire                                     axi_2_wvalid;
    wire                                     axi_2_wready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp;
    wire                                     axi_2_bvalid;
    wire                                     axi_2_bready;

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_araddr;
    wire                                     axi_3_arvalid;
    wire                                     axi_3_arready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_rdata;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_rresp;
    wire                                     axi_3_rvalid;
    wire                                     axi_3_rready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_awaddr;
    wire                                     axi_3_awvalid;
    wire                                     axi_3_awready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_wdata;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_3_wstrb;
    wire                                     axi_3_wvalid;
    wire                                     axi_3_wready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_bresp;
    wire                                     axi_3_bvalid;
    wire                                     axi_3_bready;

    ysyx_23060075_axi_xbar axi_xbar_1 (
        .clk          (clk),
        .rst          (rst),
        .axi_araddr   (axi_araddr),
        .axi_arvalid  (axi_arvalid),
        .axi_arready  (axi_arready),
        .axi_rdata    (axi_rdata),
        .axi_rresp    (axi_rresp),
        .axi_rvalid   (axi_rvalid),
        .axi_rready   (axi_rready),
        .axi_awaddr   (axi_awaddr),
        .axi_awvalid  (axi_awvalid),
        .axi_awready  (axi_awready),
        .axi_wdata    (axi_wdata),
        .axi_wstrb    (axi_wstrb),
        .axi_wvalid   (axi_wvalid),
        .axi_wready   (axi_wready),
        .axi_bresp    (axi_bresp),
        .axi_bvalid   (axi_bvalid),
        .axi_bready   (axi_bready),
        .axi_1_araddr (axi_1_araddr),
        .axi_1_arvalid(axi_1_arvalid),
        .axi_1_arready(axi_1_arready),
        .axi_1_rdata  (axi_1_rdata),
        .axi_1_rresp  (axi_1_rresp),
        .axi_1_rvalid (axi_1_rvalid),
        .axi_1_rready (axi_1_rready),
        .axi_1_awaddr (axi_1_awaddr),
        .axi_1_awvalid(axi_1_awvalid),
        .axi_1_awready(axi_1_awready),
        .axi_1_wdata  (axi_1_wdata),
        .axi_1_wstrb  (axi_1_wstrb),
        .axi_1_wvalid (axi_1_wvalid),
        .axi_1_wready (axi_1_wready),
        .axi_1_bresp  (axi_1_bresp),
        .axi_1_bvalid (axi_1_bvalid),
        .axi_1_bready (axi_1_bready),
        .axi_2_araddr (axi_2_araddr),
        .axi_2_arvalid(axi_2_arvalid),
        .axi_2_arready(axi_2_arready),
        .axi_2_rdata  (axi_2_rdata),
        .axi_2_rresp  (axi_2_rresp),
        .axi_2_rvalid (axi_2_rvalid),
        .axi_2_rready (axi_2_rready),
        .axi_2_awaddr (axi_2_awaddr),
        .axi_2_awvalid(axi_2_awvalid),
        .axi_2_awready(axi_2_awready),
        .axi_2_wdata  (axi_2_wdata),
        .axi_2_wstrb  (axi_2_wstrb),
        .axi_2_wvalid (axi_2_wvalid),
        .axi_2_wready (axi_2_wready),
        .axi_2_bresp  (axi_2_bresp),
        .axi_2_bvalid (axi_2_bvalid),
        .axi_2_bready (axi_2_bready),
        .axi_3_araddr (axi_3_araddr),
        .axi_3_arvalid(axi_3_arvalid),
        .axi_3_arready(axi_3_arready),
        .axi_3_rdata  (axi_3_rdata),
        .axi_3_rresp  (axi_3_rresp),
        .axi_3_rvalid (axi_3_rvalid),
        .axi_3_rready (axi_3_rready),
        .axi_3_awaddr (axi_3_awaddr),
        .axi_3_awvalid(axi_3_awvalid),
        .axi_3_awready(axi_3_awready),
        .axi_3_wdata  (axi_3_wdata),
        .axi_3_wstrb  (axi_3_wstrb),
        .axi_3_wvalid (axi_3_wvalid),
        .axi_3_wready (axi_3_wready),
        .axi_3_bresp  (axi_3_bresp),
        .axi_3_bvalid (axi_3_bvalid),
        .axi_3_bready (axi_3_bready)
    );

    ysyx_23060075_sram sram_1 (
        .clk        (clk),
        .rst        (rst),
        .axi_araddr (axi_1_araddr),
        .axi_arvalid(axi_1_arvalid),
        .axi_arready(axi_1_arready),
        .axi_rdata  (axi_1_rdata),
        .axi_rresp  (axi_1_rresp),
        .axi_rvalid (axi_1_rvalid),
        .axi_rready (axi_1_rready),
        .axi_awaddr (axi_1_awaddr),
        .axi_awvalid(axi_1_awvalid),
        .axi_awready(axi_1_awready),
        .axi_wdata  (axi_1_wdata),
        .axi_wstrb  (axi_1_wstrb),
        .axi_wvalid (axi_1_wvalid),
        .axi_wready (axi_1_wready),
        .axi_bresp  (axi_1_bresp),
        .axi_bvalid (axi_1_bvalid),
        .axi_bready (axi_1_bready)
    );

    ysyx_23060075_serial serial_1 (
        .clk        (clk),
        .rst        (rst),
        .axi_araddr (axi_2_araddr),
        .axi_arvalid(axi_2_arvalid),
        .axi_arready(axi_2_arready),
        .axi_rdata  (axi_2_rdata),
        .axi_rresp  (axi_2_rresp),
        .axi_rvalid (axi_2_rvalid),
        .axi_rready (axi_2_rready),
        .axi_awaddr (axi_2_awaddr),
        .axi_awvalid(axi_2_awvalid),
        .axi_awready(axi_2_awready),
        .axi_wdata  (axi_2_wdata),
        .axi_wstrb  (axi_2_wstrb),
        .axi_wvalid (axi_2_wvalid),
        .axi_wready (axi_2_wready),
        .axi_bresp  (axi_2_bresp),
        .axi_bvalid (axi_2_bvalid),
        .axi_bready (axi_2_bready)
    );

    ysyx_23060075_clint clint_3 (
        .clk        (clk),
        .rst        (rst),
        .axi_araddr (axi_3_araddr),
        .axi_arvalid(axi_3_arvalid),
        .axi_arready(axi_3_arready),
        .axi_rdata  (axi_3_rdata),
        .axi_rresp  (axi_3_rresp),
        .axi_rvalid (axi_3_rvalid),
        .axi_rready (axi_3_rready),
        .axi_awaddr (axi_3_awaddr),
        .axi_awvalid(axi_3_awvalid),
        .axi_awready(axi_3_awready),
        .axi_wdata  (axi_3_wdata),
        .axi_wstrb  (axi_3_wstrb),
        .axi_wvalid (axi_3_wvalid),
        .axi_wready (axi_3_wready),
        .axi_bresp  (axi_3_bresp),
        .axi_bvalid (axi_3_bvalid),
        .axi_bready (axi_3_bready)
    );

endmodule
