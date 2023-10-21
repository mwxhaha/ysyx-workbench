`include "config.vh"
`include "inst.vh"

module idu_funct7 (
    input wire clk,
    input wire rst,
    input wire [`ISA_WIDTH-1:0] inst,
    output wire [`INST_NUM_WIDTH-1:0] inst_srli_addi_num,
    output wire [`INST_NUM_WIDTH-1:0] inst_add_add_num,
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

    wire is_ebreak = ~(|(inst ^ `ISA_WIDTH'b0000000_00001_00000_000_00000_1110011));
    assign inst_ebreak_ebreak_num=({`INST_NUM_WIDTH{is_ebreak}}&`INST_NUM_WIDTH'd`ebreak)|({`INST_NUM_WIDTH{~is_ebreak}}&`INST_NUM_WIDTH'd`inv);

endmodule
