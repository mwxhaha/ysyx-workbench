`include "ysyx_23060075_isa.vh"
`ifndef SYNTHESIS
import "DPI-C" function void absort(input int pc);
import "DPI-C" function void ebreak(
    input int ret,
    input int pc
);
import "DPI-C" function int addr_ifetch_dpic(input int addr);
import "DPI-C" function int addr_read_dpic(input int addr);
import "DPI-C" function void addr_write_dpic(
    input int  addr,
    input byte mask,
    input int  data
);
`endif

module ysyx_23060075_cpu (
    input wire clk,
    input wire rst
);

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr;
    wire                                     mem_1_r_en;
    wire                                     mem_1_finish;
    reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask;
    wire                                     mem_2_r_en;
    wire                                     mem_2_w_en;
    wire                                     mem_2_finish;

    ysyx_23060075_core core_1 (
        .clk         (clk),
        .rst         (rst),
        .mem_1_r     (mem_1_r),
        .mem_1_addr  (mem_1_addr),
        .mem_1_r_en  (mem_1_r_en),
        .mem_1_finish(mem_1_finish),
        .mem_2_r     (mem_2_r),
        .mem_2_w     (mem_2_w),
        .mem_2_addr  (mem_2_addr),
        .mem_2_mask  (mem_2_mask),
        .mem_2_r_en  (mem_2_r_en),
        .mem_2_w_en  (mem_2_w_en),
        .mem_2_finish(mem_2_finish)
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
    ysyx_23060075_mem_ctrl mem_ctrl_1 (
        .clk          (clk),
        .rst          (rst),
        .mem_1_r      (mem_1_r),
        .mem_1_addr   (mem_1_addr),
        .mem_1_r_en   (mem_1_r_en),
        .mem_1_finish (mem_1_finish),
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
        .mem_2_r      (mem_2_r),
        .mem_2_w      (mem_2_w),
        .mem_2_addr   (mem_2_addr),
        .mem_2_mask   (mem_2_mask),
        .mem_2_r_en   (mem_2_r_en),
        .mem_2_w_en   (mem_2_w_en),
        .mem_2_finish (mem_2_finish),
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
        .axi_2_bready (axi_2_bready)
    );

    ysyx_23060075_sram sram_1 (
        .clk   (clk),
        .rst   (rst),
        .axi_araddr (axi_1_araddr),
        .axi_arvalid(axi_1_arvalid),
        .axi_arready(axi_1_arready),
        .axi_rdata  (axi_1_rdata),
        .axi_rresp  (axi_1_rresp),
        .axi_rvalid (axi_1_rvalid),
        .axi_rready (axi_1_rready),
        .axi_awaddr (axi_1_awaddr),
        .axi_awvalid(axi_1_awvalid),
        .axi_awready(axi_1_awready),
        .axi_wdata  (axi_1_wdata),
        .axi_wstrb  (axi_1_wstrb),
        .axi_wvalid (axi_1_wvalid),
        .axi_wready (axi_1_wready),
        .axi_bresp  (axi_1_bresp),
        .axi_bvalid (axi_1_bvalid),
        .axi_bready (axi_1_bready)
    );

    ysyx_23060075_sram sram_2 (
        .clk   (clk),
        .rst   (rst),
        .axi_araddr (axi_2_araddr),
        .axi_arvalid(axi_2_arvalid),
        .axi_arready(axi_2_arready),
        .axi_rdata  (axi_2_rdata),
        .axi_rresp  (axi_2_rresp),
        .axi_rvalid (axi_2_rvalid),
        .axi_rready (axi_2_rready),
        .axi_awaddr (axi_2_awaddr),
        .axi_awvalid(axi_2_awvalid),
        .axi_awready(axi_2_awready),
        .axi_wdata  (axi_2_wdata),
        .axi_wstrb  (axi_2_wstrb),
        .axi_wvalid (axi_2_wvalid),
        .axi_wready (axi_2_wready),
        .axi_bresp  (axi_2_bresp),
        .axi_bvalid (axi_2_bvalid),
        .axi_bready (axi_2_bready)
    );

`ifndef SYNTHESIS
    always @(posedge clk) begin
        if (!rst && core_1.idu_1.idu_start && core_1.inst_type == `ysyx_23060075_N) begin
            $display("invalid inst = %h at pc = %h", core_1.inst, core_1.pc);
            absort(core_1.pc);
        end
    end
    always @(posedge clk) begin
        if (!rst && core_1.csr_w_en && !(core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MEPC | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MCAUSE | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MTVEC | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MSTATUS)) begin
            $display("invalid csr = %h at pc = %h", core_1.idu_1.idu_core_1.csr_addr, core_1.pc);
            absort(core_1.pc);
        end
    end
    always @(posedge clk) begin
        if (!rst && core_1.idu_1.idu_start && core_1.inst == `ysyx_23060075_ISA_WIDTH'b0000000_00001_00000_000_00000_1110011) begin
            ebreak(core_1.idu_1.idu_core_1.gpr_1.reg_file_gpr.rf[10], core_1.pc);
        end
    end

    always @(posedge clk) begin
        if (!rst && mem_1_r_en) begin
            if ((mem_1_addr[1:0]) != 2'b00) begin
                $display("address = %h len = 4 is unalign at pc = %h", mem_1_addr, core_1.pc);
                absort(core_1.pc);
            end
        end
    end
    always @(posedge clk) begin
        if (!rst && mem_2_r_en) begin
            case (core_1.funct3[1:0])
                2'b01:
                if ((mem_2_addr[0]) != 1'b0) begin
                    $display("address = %h len = 2 is unalign at pc = %h", mem_2_addr, core_1.pc);
                    absort(core_1.pc);
                end
                2'b10:
                if ((mem_2_addr[1:0]) != 2'b00) begin
                    $display("address = %h len = 4 is unalign at pc = %h", mem_2_addr, core_1.pc);
                    absort(core_1.pc);
                end
                default: ;
            endcase
        end
    end
    always @(posedge clk) begin
        if (!rst && mem_2_w_en) begin
            case (mem_2_mask)
                `ysyx_23060075_MEM_MASK_WIDTH'b0011:
                if ((mem_2_addr[0]) != 1'b0) begin
                    $display("address = %h len = 2 is unalign at pc = %h", mem_2_addr, core_1.pc);
                    absort(core_1.pc);
                end
                `ysyx_23060075_MEM_MASK_WIDTH'b1111:
                if ((mem_2_addr[1:0]) != 2'b00) begin
                    $display("address = %h len = 4 is unalign at pc = %h", mem_2_addr, core_1.pc);
                    absort(core_1.pc);
                end
                default: ;
            endcase
        end
    end
`endif

endmodule
