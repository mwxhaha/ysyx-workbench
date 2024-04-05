`include "ysyx_23060075_isa.vh"

module ysyx_23060075_mem_ctrl (
    input wire clk,
    input wire rst,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    input  wire                                mem_1_r_en,
    output wire                                mem_1_finish,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    input  wire                                     mem_2_r_en,
    input  wire                                     mem_2_w_en,
    output wire                                     mem_2_finish,

    output wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    output wire                                axi_arvalid,
    input  wire                                axi_arready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    input  wire                                axi_rvalid,
    output wire                                axi_rready,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    output wire                                axi_awvalid,
    input  wire                                axi_awready,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_wstrb,
    output wire                                axi_wvalid,
    input  wire                                axi_wready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    input  wire                                axi_bvalid,
    output wire                                axi_bready
);

    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr;
    wire                                axi_1_arvalid;
    wire                                axi_1_arready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp;
    wire                                axi_1_rvalid;
    wire                                axi_1_rready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr;
    wire                                axi_1_awvalid;
    wire                                axi_1_awready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wstrb;
    wire                                axi_1_wvalid;
    wire                                axi_1_wready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp;
    wire                                axi_1_bvalid;
    wire                                axi_1_bready;
    ysyx_23060075_ifu_ctrl ifu_ctrl_1 (
        .clk         (clk),
        .rst         (rst),
        .mem_1_r     (mem_1_r),
        .mem_1_addr  (mem_1_addr),
        .mem_1_r_en  (mem_1_r_en),
        .mem_1_finish(mem_1_finish),
        .axi_araddr  (axi_1_araddr),
        .axi_arvalid (axi_1_arvalid),
        .axi_arready (axi_1_arready),
        .axi_rdata   (axi_1_rdata),
        .axi_rresp   (axi_1_rresp),
        .axi_rvalid  (axi_1_rvalid),
        .axi_rready  (axi_1_rready),
        .axi_awaddr  (axi_1_awaddr),
        .axi_awvalid (axi_1_awvalid),
        .axi_awready (axi_1_awready),
        .axi_wdata   (axi_1_wdata),
        .axi_wstrb   (axi_1_wstrb),
        .axi_wvalid  (axi_1_wvalid),
        .axi_wready  (axi_1_wready),
        .axi_bresp   (axi_1_bresp),
        .axi_bvalid  (axi_1_bvalid),
        .axi_bready  (axi_1_bready)
    );

    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr;
    wire                                axi_2_arvalid;
    wire                                axi_2_arready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp;
    wire                                axi_2_rvalid;
    wire                                axi_2_rready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr;
    wire                                axi_2_awvalid;
    wire                                axi_2_awready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wstrb;
    wire                                axi_2_wvalid;
    wire                                axi_2_wready;
    wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp;
    wire                                axi_2_bvalid;
    wire                                axi_2_bready;
    ysyx_23060075_lsu_ctrl lsu_ctrl_1 (
        .clk         (clk),
        .rst         (rst),
        .mem_2_r     (mem_2_r),
        .mem_2_w     (mem_2_w),
        .mem_2_addr  (mem_2_addr),
        .mem_2_mask  (mem_2_mask),
        .mem_2_r_en  (mem_2_r_en),
        .mem_2_w_en  (mem_2_w_en),
        .mem_2_finish(mem_2_finish),
        .axi_araddr  (axi_2_araddr),
        .axi_arvalid (axi_2_arvalid),
        .axi_arready (axi_2_arready),
        .axi_rdata   (axi_2_rdata),
        .axi_rresp   (axi_2_rresp),
        .axi_rvalid  (axi_2_rvalid),
        .axi_rready  (axi_2_rready),
        .axi_awaddr  (axi_2_awaddr),
        .axi_awvalid (axi_2_awvalid),
        .axi_awready (axi_2_awready),
        .axi_wdata   (axi_2_wdata),
        .axi_wstrb   (axi_2_wstrb),
        .axi_wvalid  (axi_2_wvalid),
        .axi_wready  (axi_2_wready),
        .axi_bresp   (axi_2_bresp),
        .axi_bvalid  (axi_2_bvalid),
        .axi_bready  (axi_2_bready)
    );

    ysyx_23060075_axi_arbiter axi_arbiter_1 (
        .clk          (clk),
        .rst          (rst),
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
        .axi_bready   (axi_bready)
    );
    
endmodule
