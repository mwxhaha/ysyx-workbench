`include "ysyx_23060075_isa.vh"

module ysyx_23060075_sram (
    input wire clk,
    input wire rst,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] raddr,
    output reg  [     `ysyx_23060075_ISA_WIDTH-1:0] rdata,
    input  wire                                     rvalid,
    output reg                                      rready,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] waddr,
    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] wdata,
    input  wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] wmask,
    input  wire                                     wvalid,
    output reg                                      wready
);

`ifdef SYNTHESIS
    ysyx_23060075_reg_file #(
        .ADDR_WIDTH(7),
        .DATA_WIDTH(`ysyx_23060075_ISA_WIDTH)
    ) mem_1 (
        .clk    (clk),
        .rst    (rst),
        .wdata  (wdata),
        .waddr  (waddr),
        .rdata_1(rdata),
        .raddr_1(raddr),
        .rdata_2(),
        .raddr_2(`ysyx_23060075_ISA_WIDTH'b0),
        .wen    (wvalid)
    );
`else

    reg [`ysyx_23060075_ISA_WIDTH-1:0] rcnt;
    always @(posedge clk) begin
        if (rst) rcnt <= `ysyx_23060075_ISA_WIDTH'd0;
        else if (rvalid) rcnt <= {$random} % 10 + `ysyx_23060075_ISA_WIDTH'd1;
        else if (rcnt != `ysyx_23060075_ISA_WIDTH'd0) rcnt <= rcnt - `ysyx_23060075_ISA_WIDTH'd1;
    end
    always @(posedge clk) begin
        if (rst) rready <= 1'b0;
        else if (rcnt == `ysyx_23060075_ISA_WIDTH'd1) rready <= 1'b1;
        else if (rcnt != `ysyx_23060075_ISA_WIDTH'd1) rready <= 1'b0;
    end
    always @(posedge clk) begin
        if (rst) rdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (rcnt == `ysyx_23060075_ISA_WIDTH'd1) rdata <= addr_read_dpic(raddr);
    end

    reg [`ysyx_23060075_ISA_WIDTH-1:0] wcnt;
    always @(posedge clk) begin
        if (rst) wcnt <= `ysyx_23060075_ISA_WIDTH'd0;
        else if (wvalid) wcnt <= {$random} % 10 + `ysyx_23060075_ISA_WIDTH'd1;
        else if (wcnt != `ysyx_23060075_ISA_WIDTH'd0) wcnt <= wcnt - `ysyx_23060075_ISA_WIDTH'd1;
    end
    always @(posedge clk) begin
        if (rst) rready <= 1'b0;
        else if (wcnt == `ysyx_23060075_ISA_WIDTH'd1) wready <= 1'b1;
        else if (wcnt != `ysyx_23060075_ISA_WIDTH'd1) wready <= 1'b0;
    end
    always @(posedge clk) begin
        if (rst);
        else if (wcnt == `ysyx_23060075_ISA_WIDTH'd1)
            addr_write_dpic(waddr, {4'b0000, wmask}, wdata);
    end
`endif

endmodule
