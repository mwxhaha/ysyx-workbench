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
    input  wire                                clk,
`ifdef SYNTHESIS
    output wire [`ysyx_23060075_ISA_WIDTH-1:0] pmem_2_addr,
`endif
    input  wire                                rst
);

    reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_1_addr;
    wire                                     mem_1_r_en;
    reg  [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_r;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_w;
    wire [     `ysyx_23060075_ISA_WIDTH-1:0] mem_2_addr;
    wire [`ysyx_23060075_MEM_MASK_WIDTH-1:0] mem_2_mask;
    wire                                     mem_2_r_en;
    wire                                     mem_2_w_en;

    ysyx_23060075_core core_1 (
        .clk       (clk),
        .rst       (rst),
        .mem_1_r   (mem_1_r),
        .mem_1_addr(mem_1_addr),
        .mem_1_r_en(mem_1_r_en),
        .mem_2_r   (mem_2_r),
        .mem_2_w   (mem_2_w),
        .mem_2_addr(mem_2_addr),
        .mem_2_mask(mem_2_mask),
        .mem_2_r_en(mem_2_r_en),
        .mem_2_w_en(mem_2_w_en)
    );

    ysyx_23060075_mem_ctrl mem_ctrl_1 (
        .clk        (clk),
        .rst        (rst),
        .mem_1_r    (mem_1_r),
        .mem_1_addr (mem_1_addr),
        .mem_1_r_en (mem_1_r_en),
`ifdef SYNTHESIS
        .pmem_2_addr(pmem_2_addr),
`endif
        .mem_2_r    (mem_2_r),
        .mem_2_w    (mem_2_w),
        .mem_2_addr (mem_2_addr),
        .mem_2_mask (mem_2_mask),
        .mem_2_r_en (mem_2_r_en),
        .mem_2_w_en (mem_2_w_en)
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
