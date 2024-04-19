`include "ysyx_23060075_isa.vh"

module ysyx_23060075_lsu_ctrl (
    input wire clk,
    input wire rst,

    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    input  wire                                     mem_2_r_en,
    input  wire                                     mem_2_w_en,
    output wire                                     mem_2_finish,

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
    input  wire                                axi_awready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_wdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_wstrb,
    output reg                                 axi_wvalid,
    input  wire                                axi_wready,

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_bresp,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                axi_bvalid,
    output reg                                 axi_bready
);

    // read address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_araddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_r_en) axi_araddr <= mem_2_addr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_arvalid <= 1'b0;
        else if (axi_arvalid && axi_arready) axi_arvalid <= 1'b0;
        else if (mem_2_r_en) axi_arvalid <= 1'b1;
    end

    // read data channel
    wire mem_2_r_finish;
    always @(posedge clk, posedge rst) begin
        if (rst) mem_2_r <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_rvalid && axi_rready) begin
            case (mem_2_addr[1:0])
                2'b00: mem_2_r <= axi_rdata;
                2'b01: mem_2_r <= {8'b0, axi_rdata[`ysyx_23060075_ISA_WIDTH-1:8]};
                2'b10: mem_2_r <= {16'b0, axi_rdata[`ysyx_23060075_ISA_WIDTH-1:16]};
                2'b11: mem_2_r <= {24'b0, axi_rdata[`ysyx_23060075_ISA_WIDTH-1:24]};
            endcase
        end
    end
    ysyx_23060075_pluse pluse_mem_2_r_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_rvalid && axi_rready),
        .dout(mem_2_r_finish)
    );
    always @(posedge clk, posedge rst) begin
        if (rst) axi_rready <= 1'b0;
        else if (axi_rvalid && axi_rready) axi_rready <= 1'b0;
        else if (axi_arvalid && axi_arready) axi_rready <= 1'b1;
    end

    // write address channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awaddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) axi_awaddr <= mem_2_addr;
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_awvalid <= 1'b0;
        else if (axi_awvalid && axi_awready) axi_awvalid <= 1'b0;
        else if (mem_2_w_en) axi_awvalid <= 1'b1;
    end

    // write data channel
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) begin
            case (mem_2_addr[1:0])
                2'b00: axi_wdata <= mem_2_w;
                2'b01: axi_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-9:0], 8'b0};
                2'b10: axi_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-17:0], 16'b0};
                2'b11: axi_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-25:0], 24'b0};
            endcase
        end
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wstrb <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) begin
            case (mem_2_addr[1:0])
                2'b00:
                axi_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}}, mem_2_mask
                };
                2'b01:
                axi_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[2:0],
                    1'b0
                };
                2'b10:
                axi_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[1:0],
                    2'b0
                };
                2'b11:
                axi_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[0],
                    3'b0
                };
            endcase
        end
    end
    always @(posedge clk, posedge rst) begin
        if (rst) axi_wvalid <= 1'b0;
        else if (axi_wvalid && axi_wready) axi_wvalid <= 1'b0;
        else if (mem_2_w_en) axi_wvalid <= 1'b1;
    end

    // write response channel
    wire mem_2_w_finish;
    ysyx_23060075_pluse pluse_mem_2_w_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_bvalid && axi_bready),
        .dout(mem_2_w_finish)
    );
    assign mem_2_finish = mem_2_r_finish | mem_2_w_finish;
    always @(posedge clk, posedge rst) begin
        if (rst) axi_bready <= 1'b0;
        else if (axi_bvalid && axi_bready) axi_bready <= 1'b0;
        else if (axi_awvalid && axi_awready) axi_bready <= 1'b1;
    end

endmodule
