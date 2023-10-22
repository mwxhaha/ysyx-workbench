`include "config.vh"
`include "inst.vh"

module idu_funct7 (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] inst,
    output wire [`INST_NUM_WIDTH-1:0] inst_srli_addi_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_add_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_sll_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_slt_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_sltu_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_ixor_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_srl_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_ior_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_iand_add_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_ebreak_ebreak_num
);

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_SRLI_ADDI_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_srli_addi_num (
        .out        (inst_srli_addi_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`srli,
                    `FUNCT7_WIDTH'b0100000,
                    `INST_NUM_WIDTH'd`srai
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_ADD_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_add_add_num (
        .out        (inst_add_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`add,
                    `FUNCT7_WIDTH'b0100000,
                    `INST_NUM_WIDTH'd`sub
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_SLL_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_sll_add_num (
        .out        (inst_sll_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`sll
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_SLT_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_slt_add_num (
        .out        (inst_slt_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`slt
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_SLTU_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_sltu_add_num (
        .out        (inst_sltu_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`sltu
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_IXOR_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_ixor_add_num (
        .out        (inst_ixor_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`ixor
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_SRL_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_srl_add_num (
        .out        (inst_srl_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`srl,
                    `FUNCT7_WIDTH'b0100000,
                    `INST_NUM_WIDTH'd`sra
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_IOR_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_ior_add_num (
        .out        (inst_ior_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`ior
        })
    );

    MuxKeyWithDefault #(
        .NR_KEY  (`INST_IAND_ADD_NUM_MAX),
        .KEY_LEN (`FUNCT7_WIDTH),
        .DATA_LEN(`INST_NUM_WIDTH)
    ) muxkeywithdefault_inst_iand_add_num (
        .out        (inst_iand_add_num),
        .key        (inst[25+`FUNCT7_WIDTH-1:25]),
        .default_out(`INST_NUM_WIDTH'd`inv),
        .lut        ({
                    `FUNCT7_WIDTH'b0000000,
                    `INST_NUM_WIDTH'd`iand
        })
    );

    wire is_ebreak = ~(|(inst ^ `ISA_WIDTH'b0000000_00001_00000_000_00000_1110011));
    assign inst_ebreak_ebreak_num=({`INST_NUM_WIDTH{is_ebreak}}&`INST_NUM_WIDTH'd`ebreak)|({`INST_NUM_WIDTH{~is_ebreak}}&`INST_NUM_WIDTH'd`inv);

endmodule
