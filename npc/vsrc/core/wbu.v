`include "config.vh"

module wbu (
    input  wire [`IMM_WIDTH-1:0] imm,
    input  wire [`ISA_WIDTH-1:0] snpc,
    input  wire [`ISA_WIDTH-1:0] pc_imm,
    input  wire [`ISA_WIDTH-1:0] mem_r,
    input  wire [`ISA_WIDTH-1:0] alu_result,
    output wire [`ISA_WIDTH-1:0] srd,
    input  wire                  rd_is_mem,
    input  wire                  is_lui,
    input  wire                  is_auipc,
    input  wire                  is_jal,
    input  wire                  is_jalr
);

    assign srd = is_lui ? imm : (is_auipc ? pc_imm : (is_jal | is_jalr ? snpc : (rd_is_mem ? mem_r : alu_result)));

endmodule
