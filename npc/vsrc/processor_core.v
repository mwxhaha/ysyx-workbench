`include "config.v"
import "DPI-C" function void absort_dpic(input int pc);
import "DPI-C" function void ebreak_dpic(
    input int ret,
    input int pc
);
import "DPI-C" function void pmem_read(
    input  int raddr,
    output int rdata
);
import "DPI-C" function void pmem_write(
    input int  waddr,
    input int  wdata,
    input byte wmask
);

module processor_core (
    input wire clk,
    input wire rst
);

    wire [`ISA_WIDTH-1:0] pc_out;
    reg  [`ISA_WIDTH-1:0] mem_r_1;
    wire [`ISA_WIDTH-1:0] mem_addr_1;
    wire                  mem_r_en_1;
    pc pc_1 (
        .clk    (clk),
        .rst    (rst),
        .pc_in  (pc_in),
        .pc_out (pc_out),
        .pc_w_en(pc_w_en)
    );
    assign mem_addr_1 = pc_out;
    assign mem_r_en_1 = 1'b1;

    wire [`ISA_WIDTH-1:0] inst;
    ifu ifu_1 (
        .clk  (clk),
        .rst  (rst),
        .mem_r(mem_r_1),
        .inst (inst)
    );

    wire [ `INST_NUM_WIDTH-1:0] inst_num;
    wire [`INST_TYPE_WIDTH-1:0] inst_type;
    wire [`REG_ADDR_WIDTH-1:0] rd, rs1, rs2;
    wire [`IMM_WIDTH-1:0] imm;
    idu idu_1 (
        .clk(clk),
        .rst(rst),
        .inst(inst),
        .inst_num(inst_num),
        .inst_type(inst_type),
        .rd(rd),
        .rs1(rs1),
        .rs2(rs2),
        .imm(imm)
    );

    wire [`ISA_WIDTH-1:0] src1;
    wire [`ISA_WIDTH-1:0] src2;
    gpr gpr_1 (
        .clk         (clk),
        .rst         (rst),
        .gpr_w       (srd),
        .gpr_w_addr  (rd),
        .gpr_r_1     (src1),
        .gpr_r_1_addr(rs1),
        .gpr_r_2     (src2),
        .gpr_r_2_addr(rs2),
        .gpr_w_en    (gpr_w_en)
    );

    wire [`ISA_WIDTH-1:0] alu_result;
    alu alu_1 (
        .clk       (clk),
        .rst       (rst),
        .alu_a     (alu_a),
        .alu_b     (alu_b),
        .alu_func  (alu_func),
        .alu_result(alu_result)
    );

    wire [     `ISA_WIDTH-1:0] pc_in;
    wire                       pc_w_en;
    wire [     `ISA_WIDTH-1:0] srd;
    wire                       gpr_w_en;
    reg  [     `ISA_WIDTH-1:0] mem_r_2;
    wire [     `ISA_WIDTH-1:0] mem_w_2;
    wire [     `ISA_WIDTH-1:0] mem_addr_2;
    wire                       mem_r_en_2;
    wire                       mem_w_en_2;
    wire [     `ISA_WIDTH-1:0] alu_a;
    wire [     `ISA_WIDTH-1:0] alu_b;
    wire [`ALU_FUNC_WIDTH-1:0] alu_func;
    exu exu_1 (
        .clk       (clk),
        .rst       (rst),
        .inst_num  (inst_num),
        .inst_type (inst_type),
        .imm       (imm),
        .pc_out    (pc_out),
        .pc_in     (pc_in),
        .pc_w_en   (pc_w_en),
        .src1      (src1),
        .src2      (src2),
        .srd       (srd),
        .gpr_w_en  (gpr_w_en),
        .mem_r     (mem_r_2),
        .mem_w     (mem_w_2),
        .mem_addr  (mem_addr_2),
        .mem_r_en  (mem_r_en_2),
        .mem_w_en  (mem_w_en_2),
        .alu_result(alu_result),
        .alu_a     (alu_a),
        .alu_b     (alu_b),
        .alu_func  (alu_func)
    );

    always @(posedge clk) begin
        if (!rst && (inst_num == `inv || inst_type == `N)) absort_dpic(pc_out);
    end

    always @(posedge clk) begin
        if (!rst && inst_num == `ebreak) ebreak_dpic(gpr_1.registerfile_gpr.rf[10], pc_out);
    end

    always @(*) begin
        if (!rst && mem_r_en_1) begin
            pmem_read(mem_addr_1, mem_r_1);
        end else begin
            mem_r_1 = `ISA_WIDTH'b0;
        end
    end

    always @(*) begin
        if (!rst && mem_r_en_2) begin
            pmem_read(mem_addr_2, mem_r_2);
        end else begin
            mem_r_2 = `ISA_WIDTH'b0;
        end
    end

    always @(posedge clk) begin
        if (!rst && mem_w_en_2) begin
            pmem_write(mem_addr_2, mem_w_2, 8'b00000111);
        end
    end

endmodule
