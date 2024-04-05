`include "ysyx_23060075_isa.vh"

module ysyx_23060075_mem_ctrl (
    input wire clk,
    input wire rst,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_r,
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr,
    input  wire                                mem_1_r_en,
    output wire                                mem_1_finish,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_araddr,
    output reg                                 axi_1_arvalid,
    input  wire                                axi_1_arready,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rdata,
    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_rresp,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                axi_1_rvalid,
    output reg                                 axi_1_rready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_awaddr,
    output reg                                 axi_1_awvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                axi_1_awready,
    // verilator lint_on UNUSEDSIGNAL

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_wstrb,
    output reg                                 axi_1_wvalid,
    // verilator lint_off UNUSEDSIGNAL
    input  wire                                axi_1_wready,
    // verilator lint_on UNUSEDSIGNAL

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_1_bresp,
    input  wire                                axi_1_bvalid,
    // verilator lint_on UNUSEDSIGNAL
    output reg                                 axi_1_bready,

    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask,
    input  wire                                     mem_2_r_en,
    input  wire                                     mem_2_w_en,
    output wire                                     mem_2_finish,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_araddr,
    output reg                                 axi_2_arvalid,
    input  wire                                axi_2_arready,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rdata,
    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_rresp,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                axi_2_rvalid,
    output reg                                 axi_2_rready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_awaddr,
    output reg                                 axi_2_awvalid,
    input  wire                                axi_2_awready,

    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wdata,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_wstrb,
    output reg                                 axi_2_wvalid,
    input  wire                                axi_2_wready,

    // verilator lint_off UNUSEDSIGNAL
    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] axi_2_bresp,
    // verilator lint_on UNUSEDSIGNAL
    input  wire                                axi_2_bvalid,
    output reg                                 axi_2_bready
);

    //axi 1 read address channel
    always @(posedge clk) begin
        if (rst) axi_1_araddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_1_r_en) axi_1_araddr <= mem_1_addr;
    end
    always @(posedge clk) begin
        if (rst) axi_1_arvalid <= 1'b0;
        else if (axi_1_arvalid && axi_1_arready) axi_1_arvalid <= 1'b0;
        else if (mem_1_r_en) axi_1_arvalid <= 1'b1;
    end

    //axi 1 read data channel
    always @(posedge clk) begin
        if (rst) mem_1_r <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_1_rvalid && axi_1_rready) mem_1_r <= axi_1_rdata;
    end
    ysyx_23060075_pluse pluse_mem_1_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_1_rvalid && axi_1_rready),
        .dout(mem_1_finish)
    );
    always @(posedge clk) begin
        if (rst) axi_1_rready <= 1'b0;
        else if (axi_1_rvalid && axi_1_rready) axi_1_rready <= 1'b0;
        else if (axi_1_arvalid && axi_1_arready) axi_1_rready <= 1'b1;
    end

    //axi 1 write address channel
    always @(posedge clk) begin
        if (rst) axi_1_awaddr <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk) begin
        if (rst) axi_1_awvalid <= 1'b0;
    end

    //axi 1 write data channel
    always @(posedge clk) begin
        if (rst) axi_1_wdata <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk) begin
        if (rst) axi_1_wstrb <= `ysyx_23060075_ISA_WIDTH'b0;
    end
    always @(posedge clk) begin
        if (rst) axi_1_wvalid <= 1'b0;
    end

    //axi 1 write response channel
    always @(posedge clk) begin
        if (rst) axi_1_bready <= 1'b0;
    end

    //axi 2 read address channel
    always @(posedge clk) begin
        if (rst) axi_2_araddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_r_en) axi_2_araddr <= {mem_2_addr[`ysyx_23060075_ISA_WIDTH-1:2], 2'b0};
    end
    always @(posedge clk) begin
        if (rst) axi_2_arvalid <= 1'b0;
        else if (axi_2_arvalid && axi_2_arready) axi_2_arvalid <= 1'b0;
        else if (mem_2_r_en) axi_2_arvalid <= 1'b1;
    end

    //axi 2 read data channel
    always @(posedge clk) begin
        if (rst) mem_2_r <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (axi_2_rvalid && axi_2_rready) begin
            case (mem_2_addr[1:0])
                2'b00: mem_2_r <= axi_2_rdata;
                2'b01: mem_2_r <= {8'b0, axi_2_rdata[`ysyx_23060075_ISA_WIDTH-1:8]};
                2'b10: mem_2_r <= {16'b0, axi_2_rdata[`ysyx_23060075_ISA_WIDTH-1:16]};
                2'b11: mem_2_r <= {24'b0, axi_2_rdata[`ysyx_23060075_ISA_WIDTH-1:24]};
            endcase
        end
    end
    wire mem_2_r_finish;
    ysyx_23060075_pluse pluse_mem_2_r_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_2_rvalid && axi_2_rready),
        .dout(mem_2_r_finish)
    );
    always @(posedge clk) begin
        if (rst) axi_2_rready <= 1'b0;
        else if (axi_2_rvalid && axi_2_rready) axi_2_rready <= 1'b0;
        else if (axi_2_arvalid && axi_2_arready) axi_2_rready <= 1'b1;
    end

    //axi 2 write address channel
    always @(posedge clk) begin
        if (rst) axi_2_awaddr <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) axi_2_awaddr <= {mem_2_addr[`ysyx_23060075_ISA_WIDTH-1:2], 2'b0};
    end
    always @(posedge clk) begin
        if (rst) axi_2_awvalid <= 1'b0;
        else if (axi_2_awvalid && axi_2_awready) axi_2_awvalid <= 1'b0;
        else if (mem_2_w_en) axi_2_awvalid <= 1'b1;
    end

    //axi 2 write data channel
    always @(posedge clk) begin
        if (rst) axi_2_wdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) begin
            case (mem_2_addr[1:0])
                2'b00: axi_2_wdata <= mem_2_w;
                2'b01: axi_2_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-9:0], 8'b0};
                2'b10: axi_2_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-17:0], 16'b0};
                2'b11: axi_2_wdata <= {mem_2_w[`ysyx_23060075_ISA_WIDTH-25:0], 24'b0};
            endcase
        end
    end
    always @(posedge clk) begin
        if (rst) axi_2_wstrb <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (mem_2_w_en) begin
            case (mem_2_addr[1:0])
                2'b00:
                axi_2_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}}, mem_2_mask
                };
                2'b01:
                axi_2_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[2:0],
                    1'b0
                };
                2'b10:
                axi_2_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[1:0],
                    2'b0
                };
                2'b11:
                axi_2_wstrb <= {
                    {`ysyx_23060075_ISA_WIDTH - `ysyx_23060075_MEM_MASK_WIDTH{1'b0}},
                    mem_2_mask[0],
                    3'b0
                };
            endcase
        end
    end
    always @(posedge clk) begin
        if (rst) axi_2_wvalid <= 1'b0;
        else if (axi_2_wvalid && axi_2_wready) axi_2_wvalid <= 1'b0;
        else if (mem_2_w_en) axi_2_wvalid <= 1'b1;
    end

    //axi 2 write response channel
    wire mem_2_w_finish;
    ysyx_23060075_pluse pluse_mem_2_w_finish (
        .clk (clk),
        .rst (rst),
        .din (axi_2_bvalid && axi_2_bready),
        .dout(mem_2_w_finish)
    );
    assign mem_2_finish = mem_2_r_finish | mem_2_w_finish;
    always @(posedge clk) begin
        if (rst) axi_2_bready <= 1'b0;
        else if (axi_2_bvalid && axi_2_bready) axi_2_bready <= 1'b0;
        else if (axi_2_awvalid && axi_2_awready) axi_2_bready <= 1'b1;
    end

endmodule
