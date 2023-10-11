`include "config.v"
import "DPI-C" function int ebreak_dpic();

module exu
    (
        input wire clk,rst,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        input wire [`ISA_WIDTH-1:0] pc_out,
        output wire [`ISA_WIDTH-1:0] pc_in,
        output wire pc_w_en,
        output wire [`ISA_WIDTH-1:0] srd,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        output wire gpr_w_en,
        input wire [`ISA_WIDTH-1:0] mem_r,
        output wire [`ISA_WIDTH-1:0] mem_r_addr,
        output wire [`ISA_WIDTH-1:0] mem_w,
        output wire [`ISA_WIDTH-1:0] mem_w_addr,
        output wire mem_w_en,
        input wire [`ISA_WIDTH-1:0] alu_result,
        output wire [`ISA_WIDTH-1:0] alu_a,
        output wire [`ISA_WIDTH-1:0] alu_b,
        output wire [`ALU_FUNC_WIDTH-1:0] alu_func
    );

    wire [`ISA_WIDTH-1:0] 	adder_pc_a;
    wire [`ISA_WIDTH-1:0] 	adder_pc_b;
    wire [`ISA_WIDTH-1:0] 	adder_pc_s;
    adder
        #(
            .data_len(`ISA_WIDTH)
        )
        adder_pc
        (
            .a    	( adder_pc_a     ),
            .b    	( adder_pc_b     ),
            .cin  	( 1'b0   ),
            .s    	( pc_in ),
            .cout 	(   )
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_adder_pc_a
        (
            .out(adder_pc_a),
            .key(inst_num),
            .default_out(pc_out),
            .lut({`INST_NUM_WIDTH'd`auipc,pc_out,
                  `INST_NUM_WIDTH'd`jal,pc_out,
                  `INST_NUM_WIDTH'd`jalr,src1,
                  `INST_NUM_WIDTH'd`beq,pc_out,
                  `INST_NUM_WIDTH'd`sw,pc_out,
                  `INST_NUM_WIDTH'd`addi,pc_out,
                  `INST_NUM_WIDTH'd`add,pc_out,
                  `INST_NUM_WIDTH'd`ebreak,pc_out})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_adder_pc_b
        (
            .out(adder_pc_b),
            .key(inst_num),
            .default_out(`ISA_WIDTH'd4),
            .lut({`INST_NUM_WIDTH'd`auipc,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`jal,imm,
                  `INST_NUM_WIDTH'd`jalr,imm,
                  `INST_NUM_WIDTH'd`beq,({`ISA_WIDTH{~alu_result[0]}}&imm)|({`ISA_WIDTH{alu_result[0]}}&`ISA_WIDTH'd4),
                  `INST_NUM_WIDTH'd`sw,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`addi,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`add,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'd4})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(1),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_adder_pc_s
        (
            .out(pc_in),
            .key(inst_num),
            .default_out(adder_pc_s),
            .lut({`INST_NUM_WIDTH'd`jalr,adder_pc_s&{{`ISA_WIDTH-1{1'b1}},1'b0}})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_pc_w_en
        (
            .out(pc_w_en),
            .key(inst_type),
            .default_out(1'b0),
            .lut({`INST_TYPE_WIDTH'd`R,1'b1,
                  `INST_TYPE_WIDTH'd`I,1'b1,
                  `INST_TYPE_WIDTH'd`S,1'b1,
                  `INST_TYPE_WIDTH'd`B,1'b1,
                  `INST_TYPE_WIDTH'd`U,1'b1,
                  `INST_TYPE_WIDTH'd`J,1'b1})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_srd
        (
            .out(srd),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,alu_result,
                  `INST_NUM_WIDTH'd`jal,alu_result,
                  `INST_NUM_WIDTH'd`jalr,alu_result,
                  `INST_NUM_WIDTH'd`beq,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`sw,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`addi,alu_result,
                  `INST_NUM_WIDTH'd`add,alu_result,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_gpr_w_en
        (
            .out(gpr_w_en),
            .key(inst_type),
            .default_out(1'b0),
            .lut({`INST_TYPE_WIDTH'd`R,1'b1,
                  `INST_TYPE_WIDTH'd`I,1'b1,
                  `INST_TYPE_WIDTH'd`S,1'b0,
                  `INST_TYPE_WIDTH'd`B,1'b0,
                  `INST_TYPE_WIDTH'd`U,1'b1,
                  `INST_TYPE_WIDTH'd`J,1'b1})

        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_mem_r_addr
        (
            .out(mem_r_addr),
            .key(inst_num),
            .default_out(`BASE_ADDR),
            .lut({`INST_NUM_WIDTH'd`auipc,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`jal,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`jalr,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`beq,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`sw,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`addi,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`add,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`ebreak,`BASE_ADDR})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_mem_w
        (
            .out(mem_w),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`jal,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`jalr,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`beq,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`sw,src2,
                  `INST_NUM_WIDTH'd`addi,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`add,`ISA_WIDTH'b0,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_mem_w_addr
        (
            .out(mem_w_addr),
            .key(inst_num),
            .default_out(`BASE_ADDR),
            .lut({`INST_NUM_WIDTH'd`auipc,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`jal,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`jalr,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`beq,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`sw,alu_result,
                  `INST_NUM_WIDTH'd`addi,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`add,`BASE_ADDR,
                  `INST_NUM_WIDTH'd`ebreak,`BASE_ADDR})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_TYPE_MAX),
            .KEY_LEN(`INST_TYPE_WIDTH),
            .DATA_LEN(1)
        )
        muxkeywithdefault_mem_w_en
        (
            .out(mem_w_en),
            .key(inst_type),
            .default_out(1'b0),
            .lut({`INST_TYPE_WIDTH'd`R,1'b0,
                  `INST_TYPE_WIDTH'd`I,1'b0,
                  `INST_TYPE_WIDTH'd`S,1'b1,
                  `INST_TYPE_WIDTH'd`B,1'b0,
                  `INST_TYPE_WIDTH'd`U,1'b0,
                  `INST_TYPE_WIDTH'd`J,1'b0})
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_alu_a
        (
            .out(alu_a),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,pc_out,
                  `INST_NUM_WIDTH'd`jal,pc_out,
                  `INST_NUM_WIDTH'd`jalr,pc_out,
                  `INST_NUM_WIDTH'd`beq,src1,
                  `INST_NUM_WIDTH'd`sw,src1,
                  `INST_NUM_WIDTH'd`addi,src1,
                  `INST_NUM_WIDTH'd`add,src1,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ISA_WIDTH)
        )
        muxkeywithdefault_alu_b
        (
            .out(alu_b),
            .key(inst_num),
            .default_out(`ISA_WIDTH'b0),
            .lut({`INST_NUM_WIDTH'd`auipc,imm,
                  `INST_NUM_WIDTH'd`jal,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`jal,`ISA_WIDTH'd4,
                  `INST_NUM_WIDTH'd`beq,src2,
                  `INST_NUM_WIDTH'd`sw,imm,
                  `INST_NUM_WIDTH'd`addi,imm,
                  `INST_NUM_WIDTH'd`add,src2,
                  `INST_NUM_WIDTH'd`ebreak,`ISA_WIDTH'b0})
        );
    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_NUM_MAX),
            .KEY_LEN(`INST_NUM_WIDTH),
            .DATA_LEN(`ALU_FUNC_WIDTH)
        )
        muxkeywithdefault_alu_func
        (
            .out(alu_func),
            .key(inst_num),
            .default_out(`ALU_FUNC_WIDTH'd`NO_FUNC),
            .lut({`INST_NUM_WIDTH'd`auipc,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`jal,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`jal,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`beq,`ALU_FUNC_WIDTH'd`XOR,
                  `INST_NUM_WIDTH'd`sw,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`addi,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`add,`ALU_FUNC_WIDTH'd`ADD_S,
                  `INST_NUM_WIDTH'd`ebreak,`ALU_FUNC_WIDTH'd`NO_FUNC})
        );

    always@(posedge clk)
    begin
        if (inst_num==`INST_NUM_WIDTH'd`ebreak)
            ebreak_dpic();
    end

endmodule
