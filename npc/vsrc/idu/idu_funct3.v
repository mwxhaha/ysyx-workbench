`include "config.v"

module idu_funct3
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] inst,
        input wire [`INST_NUM_WIDTH-1:0] inst_add_add_num,
        input wire [`INST_NUM_WIDTH-1:0] inst_ebreak_ebreak_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_beq_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_lb_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_sb_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_addi_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_add_num,
        output wire [`INST_NUM_WIDTH-1:0] inst_ebreak_num
    );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_BEQ_NUM_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_beq_num
        (
            .out(inst_beq_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b000,`INST_NUM_WIDTH'd`beq
                 })
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_LB_NUM_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_lb_num
        (
            .out(inst_lb_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b010,`INST_NUM_WIDTH'd`lw
                 })
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_SB_NUM_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_sb_num
        (
            .out(inst_sb_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b010,`INST_NUM_WIDTH'd`sw
                 })
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_ADDI_NUM_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_addi_num
        (
            .out(inst_addi_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b000,`INST_NUM_WIDTH'd`addi,
                     `FUNCT3_WIDTH'b011,inst_add_add_num
                 })
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_ADD_NUM_IDU_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_add_num
        (
            .out(inst_add_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b000,inst_add_add_num
                 })
        );

    MuxKeyWithDefault
        #(
            .NR_KEY(`INST_EBREAK_NUM_IDU_MAX),
            .KEY_LEN(`FUNCT3_WIDTH),
            .DATA_LEN(`INST_NUM_WIDTH)
        )
        muxkeywithdefault_inst_ebreak_num
        (
            .out(inst_ebreak_num),
            .key(inst[12+`FUNCT3_WIDTH-1:12]),
            .default_out(`INST_NUM_WIDTH'd`inv),
            .lut({
                     `FUNCT3_WIDTH'b000,inst_ebreak_ebreak_num
                 })
        );

endmodule
