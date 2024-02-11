`include "config.vh"
import "DPI-C" function void absort_dpic(input int pc);
import "DPI-C" function void ebreak_dpic(
    input int ret,
    input int pc
);
import "DPI-C" function void disable_mtrace_once_dpic();
import "DPI-C" function void pmem_read_dpic(
    input  int addr,
    output int data
);
import "DPI-C" function void pmem_write_dpic(
    input int  addr,
    input byte mask,
    input int  data
);

module cpu (
    input wire clk,
    input wire rst
);

    reg  [     `ISA_WIDTH-1:0] mem_1_r;
    wire [     `ISA_WIDTH-1:0] mem_1_addr;
    wire                       mem_1_r_en;
    reg  [     `ISA_WIDTH-1:0] mem_2_r;
    wire [     `ISA_WIDTH-1:0] mem_2_w;
    wire [     `ISA_WIDTH-1:0] mem_2_addr;
    wire [`MEM_MASK_WIDTH-1:0] mem_2_mask;
    wire                       mem_2_r_en;
    wire                       mem_2_w_en;

    core core_1 (
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
        if (!rst && core_1.idu_1.inst_type == `N) absort_dpic(core_1.ifu_1.pc);
    end

    always @(posedge clk) begin
        if (!rst && core_1.opcode == 7'b1110011 && core_1.funct3 == 3'b000 && core_1.imm == `ISA_WIDTH'b1)
            ebreak_dpic(core_1.idu_1.gpr_1.reg_file_gpr.rf[10], core_1.ifu_1.pc);
    end

    always @(*) begin
        if (!rst && mem_1_r_en) begin
            disable_mtrace_once_dpic();
            pmem_read_dpic(mem_1_addr, mem_1_r);
        end else begin
            mem_1_r = `ISA_WIDTH'b0;
        end
    end

    always @(*) begin
        if (!rst && mem_2_r_en) begin
            disable_mtrace_once_dpic();
            pmem_read_dpic(mem_2_addr, mem_2_r);
        end else begin
            mem_2_r = `ISA_WIDTH'b0;
        end
    end

    // verilator lint_off UNUSEDSIGNAL
    wire [`ISA_WIDTH-1:0] mtrace_mem_2_r;
    // verilator lint_on UNUSEDSIGNAL
    always @(negedge clk) begin
        if (!rst && mem_2_r_en) begin
            pmem_read_dpic(mem_2_addr, mtrace_mem_2_r);
        end
    end

    always @(posedge clk) begin
        if (!rst && mem_2_w_en) begin
            pmem_write_dpic(mem_2_addr, {4'b0000, mem_2_mask}, mem_2_w);
        end
    end

endmodule
