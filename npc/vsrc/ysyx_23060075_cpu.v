`include "config.vh"
import "DPI-C" function void absort(input int pc);
import "DPI-C" function void ebreak(
    input int ret,
    input int pc
);
import "DPI-C" function void disable_mtrace_once_1();
import "DPI-C" function void pmem_read(
    input  int addr,
    output int data
);
import "DPI-C" function void pmem_write(
    input int  addr,
    input byte mask,
    input int  data
);

module ysyx_23060075_cpu (
    input wire clk,
    input wire rst
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

    always @(posedge clk) begin
        if (!rst && core_1.idu_1.inst_type == `ysyx_23060075_N) absort(core_1.ifu_1.pc);
    end

    always @(posedge clk) begin
        if (!rst && core_1.opcode == 7'b1110011 && core_1.funct3 == 3'b000 && core_1.imm == `ysyx_23060075_ISA_WIDTH'b1)
            ebreak(core_1.idu_1.gpr_1.reg_file_gpr.rf[10], core_1.ifu_1.pc);
    end

    always @(*) begin
        if (!rst && mem_1_r_en) begin
            disable_mtrace_once_1();
            pmem_read(mem_1_addr, mem_1_r);
        end else begin
            mem_1_r = `ysyx_23060075_ISA_WIDTH'b0;
        end
    end

    always @(*) begin
        if (!rst && mem_2_r_en) begin
            disable_mtrace_once_1();
            pmem_read(mem_2_addr, mem_2_r);
        end else begin
            mem_2_r = `ysyx_23060075_ISA_WIDTH'b0;
        end
    end

    // verilator lint_off UNUSEDSIGNAL
    wire [`ysyx_23060075_ISA_WIDTH-1:0] mtrace_mem_2_r;
    // verilator lint_on UNUSEDSIGNAL
    always @(negedge clk) begin
        if (!rst && mem_2_r_en) begin
            pmem_read(mem_2_addr, mtrace_mem_2_r);
        end
    end

    always @(posedge clk) begin
        if (!rst && mem_2_w_en) begin
            pmem_write(mem_2_addr, {4'b0000, mem_2_mask}, mem_2_w);
        end
    end

endmodule
