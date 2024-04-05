`include "ysyx_23060075_isa.vh"
`define FREE 2'b00
`define WAIT 2'b01
`define BUSY 2'b10

module ysyx_23060075_axi_arbiter (
    input wire clk,
    input wire rst,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr,
    input  wire                                axi_1_arvalid,
    output reg                                 axi_1_arready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp,
    output reg                                 axi_1_rvalid,
    input  wire                                axi_1_rready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr,
    input  wire                                axi_1_awvalid,
    output reg                                 axi_1_awready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wstrb,
    input  wire                                axi_1_wvalid,
    output reg                                 axi_1_wready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp,
    output reg                                 axi_1_bvalid,
    input  wire                                axi_1_bready,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr,
    input  wire                                axi_2_arvalid,
    output reg                                 axi_2_arready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp,
    output reg                                 axi_2_rvalid,
    input  wire                                axi_2_rready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr,
    input  wire                                axi_2_awvalid,
    output reg                                 axi_2_awready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wstrb,
    input  wire                                axi_2_wvalid,
    output reg                                 axi_2_wready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp,
    output reg                                 axi_2_bvalid,
    input  wire                                axi_2_bready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_araddr,
    output reg                                 axi_arvalid,
    input  wire                                axi_arready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rdata,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_rresp,
    input  wire                                axi_rvalid,
    output reg                                 axi_rready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr,
    output reg                                 axi_awvalid,
    input  wire                                axi_awready,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_wstrb,
    output reg                                 axi_wvalid,
    input  wire                                axi_wready,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    input  wire                                axi_bvalid,
    output reg                                 axi_bready
);

    reg arbiter_sel;
    reg [1:0] axi_1_state;
    reg [1:0] axi_2_state;
    always @(posedge clk) begin
        if (rst) axi_1_state <= `FREE;
        else if (axi_1_arvalid && axi_1_arready || axi_1_awvalid && axi_1_awready && axi_1_wvalid && axi_1_wready)
            axi_1_state <= `BUSY;
        else if (axi_1_arvalid || axi_1_awvalid && axi_1_wvalid) axi_1_state <= `WAIT;
        else if (axi_1_rvalid && axi_1_rready || axi_1_bvalid && axi_1_bready) axi_1_state <= `FREE;
    end
    always @(posedge clk) begin
        if (rst) axi_2_state <= `FREE;
        else if (axi_2_arvalid && axi_2_arready || axi_2_awvalid && axi_2_awready && axi_2_wvalid && axi_2_wready)
            axi_2_state <= `BUSY;
        else if (axi_2_arvalid || axi_2_awvalid && axi_2_wvalid) axi_2_state <= `WAIT;
        else if (axi_2_rvalid && axi_2_rready || axi_2_bvalid && axi_2_bready) axi_2_state <= `FREE;
    end
    always @(posedge clk) begin
        if (rst) arbiter_sel <= 1'b0;
        else if (axi_1_state == `WAIT || axi_2_state == `FREE) arbiter_sel <= 1'b0;
        else if (axi_2_state == `WAIT || axi_1_state == `FREE) arbiter_sel <= 1'b1;
    end

    // read address channel
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_araddr = axi_1_araddr;
            1'b1: axi_araddr = axi_2_araddr;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_arvalid = axi_1_arvalid;
            1'b1: axi_arvalid = axi_2_arvalid;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_arready = axi_arready;
                axi_2_arready = 1'b0;
            end
            1'b1: begin
                axi_1_arready = 1'b0;
                axi_2_arready = axi_arready;
            end
        endcase
    end

    // read data channel
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_rdata = axi_rdata;
                axi_2_rdata = `ysyx_23060075_ISA_WIDTH'b0;
            end
            1'b1: begin
                axi_1_rdata = `ysyx_23060075_ISA_WIDTH'b0;
                axi_2_rdata = axi_rdata;
            end
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_rresp = axi_rresp;
                axi_2_rresp = `ysyx_23060075_ISA_WIDTH'b0;
            end
            1'b1: begin
                axi_1_rresp = `ysyx_23060075_ISA_WIDTH'b0;
                axi_2_rresp = axi_rresp;
            end
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_rvalid = axi_rvalid;
                axi_2_rvalid = 1'b0;
            end
            1'b1: begin
                axi_1_rvalid = 1'b0;
                axi_2_rvalid = axi_rvalid;
            end
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_rready = axi_1_rready;
            1'b1: axi_rready = axi_2_rready;
        endcase
    end

    // write address channel
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_awaddr = axi_1_awaddr;
            1'b1: axi_awaddr = axi_2_awaddr;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_awvalid = axi_1_awvalid;
            1'b1: axi_awvalid = axi_2_awvalid;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_awready = axi_awready;
                axi_2_awready = 1'b0;
            end
            1'b1: begin
                axi_1_awready = 1'b0;
                axi_2_awready = axi_awready;
            end
        endcase
    end

    // write data channel
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_wdata = axi_1_wdata;
            1'b1: axi_wdata = axi_2_wdata;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_wstrb = axi_1_wstrb;
            1'b1: axi_wstrb = axi_2_wstrb;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_wvalid = axi_1_wvalid;
            1'b1: axi_wvalid = axi_2_wvalid;
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_wready = axi_wready;
                axi_2_wready = 1'b0;
            end
            1'b1: begin
                axi_1_wready = 1'b0;
                axi_2_wready = axi_wready;
            end
        endcase
    end

    // write response channel
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_bresp = axi_bresp;
                axi_2_bresp = `ysyx_23060075_ISA_WIDTH'b0;
            end
            1'b1: begin
                axi_1_bresp = `ysyx_23060075_ISA_WIDTH'b0;
                axi_2_bresp = axi_bresp;
            end
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: begin
                axi_1_bvalid = axi_bvalid;
                axi_2_bvalid = 1'b0;
            end
            1'b1: begin
                axi_1_bvalid = 1'b0;
                axi_2_bvalid = axi_bvalid;
            end
        endcase
    end
    always @(*) begin
        case (arbiter_sel)
            1'b0: axi_bready = axi_1_bready;
            1'b1: axi_bready = axi_2_bready;
        endcase
    end

endmodule
