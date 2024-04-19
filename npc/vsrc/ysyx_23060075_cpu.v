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

    reg rst_reg, rst_reg2;
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            rst_reg <= 1'b1;
        end else begin
            rst_reg <= 1'b0;
        end
    end
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            rst_reg2 <= 1'b1;
        end else begin
            rst_reg2 <= rst_reg;
        end
    end

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr;
    wire                                     mem_1_r_en;
    wire                                     mem_1_finish;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask;
    wire                                     mem_2_r_en;
    wire                                     mem_2_w_en;
    wire                                     mem_2_finish;
    ysyx_23060075_core core_1 (
        .clk         (clk),
        .rst         (rst_reg2),
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

    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_araddr;
    wire                                     axi_arvalid;
    wire                                     axi_arready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rdata;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_rresp;
    wire                                     axi_rvalid;
    wire                                     axi_rready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_awaddr;
    wire                                     axi_awvalid;
    wire                                     axi_awready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_wdata;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] axi_wstrb;
    wire                                     axi_wvalid;
    wire                                     axi_wready;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] axi_bresp;
    wire                                     axi_bvalid;
    wire                                     axi_bready;
    ysyx_23060075_mem_ctrl mem_ctrl_1 (
        .clk         (clk),
        .rst         (rst_reg2),
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
        .mem_2_finish(mem_2_finish),
        .axi_araddr  (axi_araddr),
        .axi_arvalid (axi_arvalid),
        .axi_arready (axi_arready),
        .axi_rdata   (axi_rdata),
        .axi_rresp   (axi_rresp),
        .axi_rvalid  (axi_rvalid),
        .axi_rready  (axi_rready),
        .axi_awaddr  (axi_awaddr),
        .axi_awvalid (axi_awvalid),
        .axi_awready (axi_awready),
        .axi_wdata   (axi_wdata),
        .axi_wstrb   (axi_wstrb),
        .axi_wvalid  (axi_wvalid),
        .axi_wready  (axi_wready),
        .axi_bresp   (axi_bresp),
        .axi_bvalid  (axi_bvalid),
        .axi_bready  (axi_bready)
    );

    ysyx_23060075_sram sram_1 (
        .clk        (clk),
        .rst        (rst_reg2),
        .axi_araddr (axi_araddr),
        .axi_arvalid(axi_arvalid),
        .axi_arready(axi_arready),
        .axi_rdata  (axi_rdata),
        .axi_rresp  (axi_rresp),
        .axi_rvalid (axi_rvalid),
        .axi_rready (axi_rready),
        .axi_awaddr (axi_awaddr),
        .axi_awvalid(axi_awvalid),
        .axi_awready(axi_awready),
        .axi_wdata  (axi_wdata),
        .axi_wstrb  (axi_wstrb),
        .axi_wvalid (axi_wvalid),
        .axi_wready (axi_wready),
        .axi_bresp  (axi_bresp),
        .axi_bvalid (axi_bvalid),
        .axi_bready (axi_bready)
    );

`ifndef SYNTHESIS
    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (core_1.idu_1.idu_start && core_1.inst_type == `ysyx_23060075_N) begin
            $display("invalid inst = %h at pc = %h", core_1.inst, core_1.pc);
            absort(core_1.pc);
        end
    end
    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (core_1.csr_w_en && !(core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MEPC | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MCAUSE | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MTVEC | core_1.idu_1.idu_core_1.csr_addr == `ysyx_23060075_CSR_ADDR_MSTATUS)) begin
            $display("invalid csr = %h at pc = %h", core_1.idu_1.idu_core_1.csr_addr, core_1.pc);
            absort(core_1.pc);
        end
    end
    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (core_1.idu_1.idu_start && core_1.inst == `ysyx_23060075_ISA_WIDTH'b0000000_00001_00000_000_00000_1110011) begin
            ebreak(core_1.idu_1.idu_core_1.gpr_1.reg_file_gpr.rf[10], core_1.pc);
        end
    end

    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (mem_1_r_en) begin
            if ((mem_1_addr[1:0]) != 2'b00) begin
                $display("address = %h len = 4 is unalign at pc = %h", mem_1_addr, core_1.pc);
                absort(core_1.pc);
            end
        end
    end
    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (mem_2_r_en) begin
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
    always @(posedge clk, posedge rst_reg2) begin
        if (rst_reg2);
        else if (mem_2_w_en) begin
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
