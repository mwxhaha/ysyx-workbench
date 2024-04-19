`include "ysyx_23060075_isa.vh"
`define FREE 2'b00
`define WAIT 2'b01
`define BUSY 2'b10

module ysyx_23060075_axi_arbiter (
    input wire clk,
    input wire rst,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr,
    input  wire                                     axi_1_arvalid,
    output wire                                     axi_1_arready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp,
    output wire                                     axi_1_rvalid,
    input  wire                                     axi_1_rready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr,
    input  wire                                     axi_1_awvalid,
    output wire                                     axi_1_awready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_1_wstrb,
    input  wire                                     axi_1_wvalid,
    output wire                                     axi_1_wready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp,
    output wire                                     axi_1_bvalid,
    input  wire                                     axi_1_bready,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr,
    input  wire                                     axi_2_arvalid,
    output wire                                     axi_2_arready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp,
    output wire                                     axi_2_rvalid,
    input  wire                                     axi_2_rready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr,
    input  wire                                     axi_2_awvalid,
    output wire                                     axi_2_awready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_2_wstrb,
    input  wire                                     axi_2_wvalid,
    output wire                                     axi_2_wready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp,
    output wire                                     axi_2_bvalid,
    input  wire                                     axi_2_bready,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    output wire                                     axi_arvalid,
    input  wire                                     axi_arready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    input  wire                                     axi_rvalid,
    output wire                                     axi_rready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    output wire                                     axi_awvalid,
    input  wire                                     axi_awready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb,
    output wire                                     axi_wvalid,
    input  wire                                     axi_wready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    input  wire                                     axi_bvalid,
    output wire                                     axi_bready
);

    reg arbiter_sel;
    reg [1:0] axi_1_state;
    reg [1:0] axi_2_state;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_1_state <= `FREE;
        else if (axi_1_arvalid && axi_1_arready || axi_1_awvalid && axi_1_awready && axi_1_wvalid && axi_1_wready)
            axi_1_state <= `BUSY;
        else if (axi_1_arvalid || axi_1_awvalid && axi_1_wvalid) axi_1_state <= `WAIT;
        else if (axi_1_rvalid && axi_1_rready || axi_1_bvalid && axi_1_bready) axi_1_state <= `FREE;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_2_state <= `FREE;
        else if (axi_2_arvalid && axi_2_arready || axi_2_awvalid && axi_2_awready && axi_2_wvalid && axi_2_wready)
            axi_2_state <= `BUSY;
        else if (axi_2_arvalid || axi_2_awvalid && axi_2_wvalid) axi_2_state <= `WAIT;
        else if (axi_2_rvalid && axi_2_rready || axi_2_bvalid && axi_2_bready) axi_2_state <= `FREE;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) arbiter_sel <= 1'b0;
        else if (axi_1_state == `WAIT || axi_2_state == `FREE) arbiter_sel <= 1'b0;
        else if (axi_2_state == `WAIT || axi_1_state == `FREE) arbiter_sel <= 1'b1;
    end

    wire [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_1_mtos = {
        axi_1_araddr,
        axi_1_arvalid,
        axi_1_rready,
        axi_1_awaddr,
        axi_1_awvalid,
        axi_1_wdata,
        axi_1_wstrb,
        axi_1_wvalid,
        axi_1_bready
    };
    reg [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_1_stom;
    assign axi_1_arready = axi_1_stom[`ysyx_23060075_ISA_WIDTH*3+4];
    assign axi_1_rdata   = axi_1_stom[`ysyx_23060075_ISA_WIDTH*3+3:`ysyx_23060075_ISA_WIDTH*2+4];
    assign axi_1_rresp   = axi_1_stom[`ysyx_23060075_ISA_WIDTH*2+3:`ysyx_23060075_ISA_WIDTH+4];
    assign axi_1_rvalid  = axi_1_stom[`ysyx_23060075_ISA_WIDTH+3];
    assign axi_1_awready = axi_1_stom[`ysyx_23060075_ISA_WIDTH+2];
    assign axi_1_wready  = axi_1_stom[`ysyx_23060075_ISA_WIDTH+1];
    assign axi_1_bresp   = axi_1_stom[`ysyx_23060075_ISA_WIDTH:1];
    assign axi_1_bvalid  = axi_1_stom[0];

    wire [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_2_mtos = {
        axi_2_araddr,
        axi_2_arvalid,
        axi_2_rready,
        axi_2_awaddr,
        axi_2_awvalid,
        axi_2_wdata,
        axi_2_wstrb,
        axi_2_wvalid,
        axi_2_bready
    };
    reg [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_2_stom;
    assign axi_2_arready = axi_2_stom[`ysyx_23060075_ISA_WIDTH*3+4];
    assign axi_2_rdata   = axi_2_stom[`ysyx_23060075_ISA_WIDTH*3+3:`ysyx_23060075_ISA_WIDTH*2+4];
    assign axi_2_rresp   = axi_2_stom[`ysyx_23060075_ISA_WIDTH*2+3:`ysyx_23060075_ISA_WIDTH+4];
    assign axi_2_rvalid  = axi_2_stom[`ysyx_23060075_ISA_WIDTH+3];
    assign axi_2_awready = axi_2_stom[`ysyx_23060075_ISA_WIDTH+2];
    assign axi_2_wready  = axi_2_stom[`ysyx_23060075_ISA_WIDTH+1];
    assign axi_2_bresp   = axi_2_stom[`ysyx_23060075_ISA_WIDTH:1];
    assign axi_2_bvalid  = axi_2_stom[0];

    reg [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_mtos;
    assign axi_araddr  = axi_mtos[`ysyx_23060075_ISA_WIDTH*3+`ysyx_23060075_MEM_MASK_WIDTH+4:`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+5];
    assign axi_arvalid = axi_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+4];
    assign axi_rready = axi_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_awaddr  = axi_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+2:`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_awvalid = axi_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_wdata   = axi_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+1:`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_wstrb = axi_mtos[`ysyx_23060075_MEM_MASK_WIDTH+1:2];
    assign axi_wvalid = axi_mtos[1];
    assign axi_bready = axi_mtos[0];
    wire [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_stom = {
        axi_arready,
        axi_rdata,
        axi_rresp,
        axi_rvalid,
        axi_awready,
        axi_wready,
        axi_bresp,
        axi_bvalid
    };

    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_stom = axi_stom;
                axi_2_stom = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_mtos   = axi_1_mtos;
            end
            1'b1: begin
                axi_1_stom = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_2_stom = axi_stom;
                axi_mtos   = axi_2_mtos;
            end
            default: begin
                axi_1_stom = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_2_stom = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_mtos   = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
            end
        endcase
    end

endmodule
