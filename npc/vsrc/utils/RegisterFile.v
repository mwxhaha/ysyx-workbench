module RegisterFile #(
    parameter ADDR_WIDTH = 1,
    parameter DATA_WIDTH = 1
) (
    input  wire                  clk,
    input  wire [DATA_WIDTH-1:0] wdata,
    input  wire [ADDR_WIDTH-1:0] waddr,
    output wire [DATA_WIDTH-1:0] rdata_1,
    input  wire [ADDR_WIDTH-1:0] raddr_1,
    output wire [DATA_WIDTH-1:0] rdata_2,
    input  wire [ADDR_WIDTH-1:0] raddr_2,
    input  wire                  wen
);

    reg [DATA_WIDTH-1:0] rf[0:2**ADDR_WIDTH-1];
    always @(posedge clk) begin
        if (wen) rf[waddr] <= wdata;
    end

    assign rdata_1 = rf[raddr_1];
    assign rdata_2 = rf[raddr_2];

endmodule
