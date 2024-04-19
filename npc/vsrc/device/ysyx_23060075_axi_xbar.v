`include "ysyx_23060075_isa.vh"
`define FREE 2'b00
`define WAIT 2'b01
`define BUSY 2'b10

module ysyx_23060075_axi_xbar (
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
    input  wire                                     axi_bready,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr,
    output wire                                     axi_1_arvalid,
    input  wire                                     axi_1_arready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp,
    input  wire                                     axi_1_rvalid,
    output wire                                     axi_1_rready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr,
    output wire                                     axi_1_awvalid,
    input  wire                                     axi_1_awready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_1_wstrb,
    output wire                                     axi_1_wvalid,
    input  wire                                     axi_1_wready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp,
    input  wire                                     axi_1_bvalid,
    output wire                                     axi_1_bready,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr,
    output wire                                     axi_2_arvalid,
    input  wire                                     axi_2_arready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp,
    input  wire                                     axi_2_rvalid,
    output wire                                     axi_2_rready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr,
    output wire                                     axi_2_awvalid,
    input  wire                                     axi_2_awready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_2_wstrb,
    output wire                                     axi_2_wvalid,
    input  wire                                     axi_2_wready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp,
    input  wire                                     axi_2_bvalid,
    output wire                                     axi_2_bready,

    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_araddr,
    output wire                                     axi_3_arvalid,
    input  wire                                     axi_3_arready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_rdata,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_rresp,
    input  wire                                     axi_3_rvalid,
    output wire                                     axi_3_rready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_awaddr,
    output wire                                     axi_3_awvalid,
    input  wire                                     axi_3_awready,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_wdata,
    output wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_3_wstrb,
    output wire                                     axi_3_wvalid,
    input  wire                                     axi_3_wready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_3_bresp,
    input  wire                                     axi_3_bvalid,
    output wire                                     axi_3_bready
);

    reg [1:0] xbar_sel;
    always @(posedge clk, posedge rst) begin
        if (rst) xbar_sel <= 2'b00;
        else if(axi_araddr<`ysyx_23060075_ISA_WIDTH'h88000000 &&axi_arvalid||axi_awaddr<`ysyx_23060075_ISA_WIDTH'h88000000 &&axi_awvalid)
            xbar_sel <= 2'b01;
        else if(axi_araddr>=`ysyx_23060075_ISA_WIDTH'ha00003f8&& axi_araddr<`ysyx_23060075_ISA_WIDTH'ha0000400&&axi_arvalid||axi_awaddr>=`ysyx_23060075_ISA_WIDTH'ha00003f8&& axi_awaddr<`ysyx_23060075_ISA_WIDTH'ha0000400&&axi_awvalid)
            xbar_sel <= 2'b10;
        // else if(axi_araddr>=`ysyx_23060075_ISA_WIDTH'ha0000048&& axi_araddr<`ysyx_23060075_ISA_WIDTH'ha0000050&&axi_arvalid||axi_awaddr>=`ysyx_23060075_ISA_WIDTH'ha0000048&& axi_awaddr<`ysyx_23060075_ISA_WIDTH'ha0000500&&axi_awvalid)
        //     xbar_sel <= 2'b11;
        else if (axi_rvalid && axi_rready || axi_bvalid && axi_bready) xbar_sel <= 2'b00;
    end

    wire [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_mtos = {
        axi_araddr,
        axi_arvalid,
        axi_rready,
        axi_awaddr,
        axi_awvalid,
        axi_wdata,
        axi_wstrb,
        axi_wvalid,
        axi_bready
    };
    reg [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_stom;
    assign axi_arready = axi_stom[`ysyx_23060075_ISA_WIDTH*3+4];
    assign axi_rdata   = axi_stom[`ysyx_23060075_ISA_WIDTH*3+3:`ysyx_23060075_ISA_WIDTH*2+4];
    assign axi_rresp   = axi_stom[`ysyx_23060075_ISA_WIDTH*2+3:`ysyx_23060075_ISA_WIDTH+4];
    assign axi_rvalid  = axi_stom[`ysyx_23060075_ISA_WIDTH+3];
    assign axi_awready = axi_stom[`ysyx_23060075_ISA_WIDTH+2];
    assign axi_wready  = axi_stom[`ysyx_23060075_ISA_WIDTH+1];
    assign axi_bresp   = axi_stom[`ysyx_23060075_ISA_WIDTH:1];
    assign axi_bvalid  = axi_stom[0];

    reg [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_1_mtos;
    assign axi_1_araddr  = axi_1_mtos[`ysyx_23060075_ISA_WIDTH*3+`ysyx_23060075_MEM_MASK_WIDTH+4:`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+5];
    assign axi_1_arvalid = axi_1_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+4];
    assign axi_1_rready = axi_1_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_1_awaddr  = axi_1_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+2:`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_1_awvalid = axi_1_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_1_wdata   = axi_1_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+1:`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_1_wstrb = axi_1_mtos[`ysyx_23060075_MEM_MASK_WIDTH+1:2];
    assign axi_1_wvalid = axi_1_mtos[1];
    assign axi_1_bready = axi_1_mtos[0];
    wire [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_1_stom = {
        axi_1_arready,
        axi_1_rdata,
        axi_1_rresp,
        axi_1_rvalid,
        axi_1_awready,
        axi_1_wready,
        axi_1_bresp,
        axi_1_bvalid
    };

    reg [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_2_mtos;
    assign axi_2_araddr  = axi_2_mtos[`ysyx_23060075_ISA_WIDTH*3+`ysyx_23060075_MEM_MASK_WIDTH+4:`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+5];
    assign axi_2_arvalid = axi_2_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+4];
    assign axi_2_rready = axi_2_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_2_awaddr  = axi_2_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+2:`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_2_awvalid = axi_2_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_2_wdata   = axi_2_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+1:`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_2_wstrb = axi_2_mtos[`ysyx_23060075_MEM_MASK_WIDTH+1:2];
    assign axi_2_wvalid = axi_2_mtos[1];
    assign axi_2_bready = axi_2_mtos[0];
    wire [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_2_stom = {
        axi_2_arready,
        axi_2_rdata,
        axi_2_rresp,
        axi_2_rvalid,
        axi_2_awready,
        axi_2_wready,
        axi_2_bresp,
        axi_2_bvalid
    };

    reg [`ysyx_23060075_AXI_MTOS_WIDTH-1:0] axi_3_mtos;
    assign axi_3_araddr  = axi_3_mtos[`ysyx_23060075_ISA_WIDTH*3+`ysyx_23060075_MEM_MASK_WIDTH+4:`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+5];
    assign axi_3_arvalid = axi_3_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+4];
    assign axi_3_rready = axi_3_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_3_awaddr  = axi_3_mtos[`ysyx_23060075_ISA_WIDTH*2+`ysyx_23060075_MEM_MASK_WIDTH+2:`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+3];
    assign axi_3_awvalid = axi_3_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_3_wdata   = axi_3_mtos[`ysyx_23060075_ISA_WIDTH+`ysyx_23060075_MEM_MASK_WIDTH+1:`ysyx_23060075_MEM_MASK_WIDTH+2];
    assign axi_3_wstrb = axi_3_mtos[`ysyx_23060075_MEM_MASK_WIDTH+1:2];
    assign axi_3_wvalid = axi_3_mtos[1];
    assign axi_3_bready = axi_3_mtos[0];
    wire [`ysyx_23060075_AXI_STOM_WIDTH-1:0] axi_3_stom = {
        axi_3_arready,
        axi_3_rdata,
        axi_3_rresp,
        axi_3_rvalid,
        axi_3_awready,
        axi_3_wready,
        axi_3_bresp,
        axi_3_bvalid
    };

    always @(*) begin
        case (xbar_sel)
            2'b00: begin
                axi_stom   = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_1_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_2_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_3_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
            end
            2'b01: begin
                axi_stom   = axi_1_stom;
                axi_1_mtos = axi_mtos;
                axi_2_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_3_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
            end
            2'b10: begin
                axi_stom   = axi_2_stom;
                axi_1_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_2_mtos = axi_mtos;
                axi_3_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
            end
            2'b11: begin
                axi_stom   = axi_3_stom;
                axi_1_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_2_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_3_mtos = axi_mtos;
            end
            default: begin
                axi_stom   = {`ysyx_23060075_AXI_STOM_WIDTH{1'b0}};
                axi_1_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_2_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
                axi_3_mtos = {`ysyx_23060075_AXI_MTOS_WIDTH{1'b0}};
            end
        endcase
    end

endmodule
