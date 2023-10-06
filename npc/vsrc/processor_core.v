module processor_core
    (
        input wire clk,rst,
        input wire [`ISA_WIDTH-1:0] mem_r_1,
        output wire [`ISA_WIDTH-1:0] mem_r_1_addr,
        input wire [`ISA_WIDTH-1:0] mem_r_2,
        output wire [`ISA_WIDTH-1:0] mem_r_2_addr,
        output wire [`ISA_WIDTH-1:0] mem_w,
        output wire [`ISA_WIDTH-1:0] mem_w_addr,
        output wire mem_w_en
    );

    wire [`ISA_WIDTH-1:0] 	pc_out;
    pc pc_1
       (
           .clk     	( clk      ),
           .rst     	( rst      ),
           .pc_in   	( pc_in    ),
           .pc_out  	( pc_out   ),
           .pc_w_en 	( pc_w_en  )
       );
    assign mem_r_1_addr=pc_out;

    wire [`ISA_WIDTH-1:0] 	inst;
    ifu ifu_1
        (
            .clk   	( clk    ),
            .rst   	( rst    ),
            .mem_r 	( mem_r_1  ),
            .inst  	( inst   )
        );

    wire [`INST_NUM_WIDTH-1:0] inst_num;
    wire [`INST_TYPE_WIDTH-1:0] inst_type;
    wire [`REG_ADDR_WIDTH-1:0] rd,rs1,rs2;
    wire [`IMM_WIDTH-1:0] imm;
    idu idu_1
        (
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

    wire [`ISA_WIDTH-1:0]      	src1;
    wire [`ISA_WIDTH-1:0]      	src2;
    gpr gpr_1
        (
            .clk          	( clk           ),
            .rst          	( rst           ),
            .gpr_w        	( srd        ),
            .gpr_w_addr   	( rd    ),
            .gpr_r_1      	( src1       ),
            .gpr_r_1_addr 	( rs1  ),
            .gpr_r_2      	( src2       ),
            .gpr_r_2_addr 	( rs2  ),
            .gpr_w_en     	( gpr_w_en      )
        );

    wire [`ISA_WIDTH-1:0] 	alu_result;
    alu alu_1
    (
        .clk     	    ( clk      ),
        .rst     	    ( rst      ),
        .alu_a      	( alu_a       ),
        .alu_b      	( alu_b       ),
        .alu_func   	( alu_func    ),
        .alu_result 	( alu_result  )
    );

    wire [`ISA_WIDTH-1:0]       	pc_in;
    wire                        	pc_w_en;
    wire [`ISA_WIDTH-1:0]       	srd;
    wire                        	gpr_w_en;
    wire [`ISA_WIDTH-1:0]           alu_a;
    wire [`ISA_WIDTH-1:0]           alu_b;
    wire [`ALU_FUNC_WIDTH-1:0]      alu_func;
    exu exu_1
    (
            .clk        	( clk         ),
            .rst        	( rst         ),
            .inst_num   	( inst_num    ),
            .inst_type  	( inst_type   ),
            .imm        	( imm         ),
            .pc_out     	( pc_out      ),
            .pc_in      	( pc_in       ),
            .pc_w_en    	( pc_w_en     ),
            .srd        	( srd         ),
            .src1       	( src1        ),
            .src2       	( src2        ),
            .gpr_w_en   	( gpr_w_en    ),
            .mem_r      	( mem_r_2       ),
            .mem_r_addr 	( mem_r_2_addr  ),
            .mem_w      	( mem_w       ),
            .mem_w_addr 	( mem_w_addr  ),
            .mem_w_en   	( mem_w_en    ),
            .alu_result 	( alu_result  ),
            .alu_a      	( alu_a       ),
            .alu_b 	        ( alu_b  ),
            .alu_func   	( alu_func    )
        );

endmodule
