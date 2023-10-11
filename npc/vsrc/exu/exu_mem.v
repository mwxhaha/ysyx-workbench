`include "config.v"

module exu_mem
    (
        input wire clk,rst,
        input wire [`INST_NUM_WIDTH-1:0] inst_num,
        input wire [`INST_TYPE_WIDTH-1:0] inst_type,
        input wire [`IMM_WIDTH-1:0] imm,
        input wire [`ISA_WIDTH-1:0] pc_out,
        input wire [`ISA_WIDTH-1:0] src1,
        input wire [`ISA_WIDTH-1:0] src2,
        input wire [`ISA_WIDTH-1:0] mem_r,
        output wire [`ISA_WIDTH-1:0] mem_r_addr,
        output wire [`ISA_WIDTH-1:0] mem_w,
        output wire [`ISA_WIDTH-1:0] mem_w_addr,
        output wire mem_w_en,
        input wire [`ISA_WIDTH-1:0] alu_result
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

endmodule
