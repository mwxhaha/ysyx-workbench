`include "config.vh"

module ysyx_23060075_csr (
    input wire clk,
    input wire rst,

    input  wire [     `ysyx_23060075_ISA_WIDTH-1:0] csr_w,
    output wire [     `ysyx_23060075_ISA_WIDTH-1:0] csr_r,
    input  wire [`ysyx_23060075_CSR_ADDR_WIDTH-1:0] csr_addr,
    input  wire                                     csr_w_en
);

    wire [`ysyx_23060075_ISA_WIDTH-1:0] mepc;
    ysyx_23060075_register #(
        .WIDTH    (`ysyx_23060075_ISA_WIDTH),
        .RESET_VAL(`ysyx_23060075_ISA_WIDTH'b0)
    ) register_mepc (
        .clk (clk),
        .rst (rst),
        .din (csr_w),
        .dout(mepc),
        .wen (csr_w_en & csr_addr == `ysyx_23060075_CSR_ADDR_MEPC)
    );
    wire [`ysyx_23060075_ISA_WIDTH-1:0] mcause;
    ysyx_23060075_register #(
        .WIDTH    (`ysyx_23060075_ISA_WIDTH),
        .RESET_VAL(`ysyx_23060075_ISA_WIDTH'b0)
    ) register_mcause (
        .clk (clk),
        .rst (rst),
        .din (csr_w),
        .dout(mcause),
        .wen (csr_w_en & csr_addr == `ysyx_23060075_CSR_ADDR_MCAUSE)
    );
    wire [`ysyx_23060075_ISA_WIDTH-1:0] mtvec;
    ysyx_23060075_register #(
        .WIDTH    (`ysyx_23060075_ISA_WIDTH),
        .RESET_VAL(`ysyx_23060075_ISA_WIDTH'b0)
    ) register_mtvec (
        .clk (clk),
        .rst (rst),
        .din (csr_w),
        .dout(mtvec),
        .wen (csr_w_en & csr_addr == `ysyx_23060075_CSR_ADDR_MTVEC)
    );
    wire [`ysyx_23060075_ISA_WIDTH-1:0] mstatus;
    ysyx_23060075_register #(
        .WIDTH    (`ysyx_23060075_ISA_WIDTH),
        .RESET_VAL(`ysyx_23060075_ISA_WIDTH'b0)
    ) register_mstatus (
        .clk (clk),
        .rst (rst),
        .din (csr_w),
        .dout(mstatus),
        .wen (csr_w_en & csr_addr == `ysyx_23060075_CSR_ADDR_MSTATUS)
    );

    ysyx_23060075_mux_def #(
        .NR_KEY  (4),
        .KEY_LEN (`ysyx_23060075_CSR_ADDR_WIDTH),
        .DATA_LEN(`ysyx_23060075_ISA_WIDTH)
    ) mux_def_csr_r (
        .out(csr_r),
        .key(csr_addr),
        .default_out(0),
        .lut({
            `ysyx_23060075_CSR_ADDR_MEPC,
            mepc,
            `ysyx_23060075_CSR_ADDR_MCAUSE,
            mcause,
            `ysyx_23060075_CSR_ADDR_MTVEC,
            mtvec,
            `ysyx_23060075_CSR_ADDR_MSTATUS,
            mstatus
        })
    );

endmodule
