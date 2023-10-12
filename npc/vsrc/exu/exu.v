`include "config.v"

module exu
    (
        input wire clk,rst,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        input wire [`ISA_WIDTH-1:0] pc_out,
        output wire [`ISA_WIDTH-1:0] pc_in,
        output wire pc_w_en,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        output wire [`ISA_WIDTH-1:0] srd,
        output wire gpr_w_en,
        input wire [`ISA_WIDTH-1:0] mem_r,
        output wire [`ISA_WIDTH-1:0] mem_w,
        output wire [`ISA_WIDTH-1:0] mem_addr,
        output wire mem_r_en,
        output wire mem_w_en,
        input wire [`ISA_WIDTH-1:0] alu_result,
        output wire [`ISA_WIDTH-1:0] alu_a,
        output wire [`ISA_WIDTH-1:0] alu_b,
        output wire [`ALU_FUNC_WIDTH-1:0] alu_func
    );

    exu_pc exu_pc_1
           (
               .clk(clk),
               .rst(rst),
               .inst_num(inst_num),
               .inst_type(inst_type),
               .imm(imm),
               .pc_out(pc_out),
               .pc_in(pc_in),
               .pc_w_en(pc_w_en),
               .src1(src1),
               .src2(src2),
               .mem_r(mem_r),
               .alu_result(alu_result)
           );

    exu_gpr exu_gpr_1
            (
                .clk(clk),
                .rst(rst),
                .inst_num(inst_num),
                .inst_type(inst_type),
                .imm(imm),
                .pc_out(pc_out),
                .src1(src1),
                .src2(src2),
                .srd(srd),
                .gpr_w_en(gpr_w_en),
                .mem_r(mem_r),
                .alu_result(alu_result)
            );

    exu_mem exu_mem_1
            (
                .clk(clk),
                .rst(rst),
                .inst_num(inst_num),
                .inst_type(inst_type),
                .imm(imm),
                .pc_out(pc_out),
                .src1(src1),
                .src2(src2),
                .mem_r(mem_r),
                .mem_w(mem_w),
                .mem_addr(mem_addr),
                .mem_r_en(mem_r_en),
                .mem_w_en(mem_w_en),
                .alu_result(alu_result)
            );

    exu_alu exu_alu_1
            (
                .clk(clk),
                .rst(rst),
                .inst_num(inst_num),
                .inst_type(inst_type),
                .imm(imm),
                .pc_out(pc_out),
                .src1(src1),
                .src2(src2),
                .mem_r(mem_r),
                .alu_result(alu_result),
                .alu_a(alu_a),
                .alu_b(alu_b),
                .alu_func(alu_func)
            );

endmodule
