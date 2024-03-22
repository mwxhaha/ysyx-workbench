`include "ysyx_23060075_isa.vh"

module ysyx_23060075_sram (
    input wire clk,
    input wire rst,

    input  wire [`ysyx_23060075_ISA_WIDTH-1:0] raddr,
    output reg  [`ysyx_23060075_ISA_WIDTH-1:0] rdata,
    input  wire                                rvalid,
    output reg                                 rready
);

`ifdef SYNTHESIS
    ysyx_23060075_reg_file #(
        .ADDR_WIDTH(7),
        .DATA_WIDTH(`ysyx_23060075_ISA_WIDTH)
    ) mem_1 (
        .clk    (clk),
        .rst    (rst),
        .wdata  (`ysyx_23060075_ISA_WIDTH'b0),
        .waddr  (`ysyx_23060075_ISA_WIDTH'b0),
        .rdata_1(rdata),
        .raddr_1(raddr),
        .rdata_2(),
        .raddr_2(`ysyx_23060075_ISA_WIDTH'b0),
        .wen    (1'b0)
    );
`else
    reg [`ysyx_23060075_ISA_WIDTH-1:0] cnt;
    always @(posedge clk) begin
        if (rst) cnt <= `ysyx_23060075_ISA_WIDTH'd0;
        else if (rvalid) cnt <= {$random} % 10 + `ysyx_23060075_ISA_WIDTH'd1;
        else if (cnt != `ysyx_23060075_ISA_WIDTH'd0) cnt <= cnt - `ysyx_23060075_ISA_WIDTH'd1;
    end
    always @(posedge clk) begin
        if (rst) rready <= 1'b0;
        else if (cnt == `ysyx_23060075_ISA_WIDTH'd1) rready <= 1'b1;
        else if (cnt != `ysyx_23060075_ISA_WIDTH'd1) rready <= 1'b0;
    end
    always @(posedge clk) begin
        if (rst) rdata <= `ysyx_23060075_ISA_WIDTH'b0;
        else if (cnt == `ysyx_23060075_ISA_WIDTH'd1) rdata <= addr_read_dpic(raddr);
    end
`endif

endmodule
