module alu
    #(
         parameter data_len=4
     )
     (
         input wire [data_len-1:0] a,b,
         input wire [2:0] func,
         output reg [data_len-1:0] result
     );

    wire [data_len-1:0] result_adder_suber_i;
    reg add_or_sub;
    wire zero,overflow;

    always@(*)
    begin
        case (func)
            3'b000:
                add_or_sub=1'b0;
            3'b001:
                add_or_sub=1'b1;
            3'b110:
                add_or_sub=1'b1;
            3'b111:
                add_or_sub=1'b1;
            default:
                add_or_sub=1'b0;
        endcase
    end

    always@(*)
    begin
        case (func)
            3'b000:
                result=result_adder_suber_i;
            3'b001:
                result=result_adder_suber_i;
            3'b010:
                result=~a;
            3'b011:
                result=a&b;
            3'b100:
                result=a|b;
            3'b101:
                result=a^b;
            3'b110:
                result=result_adder_suber_i[data_len-1]^overflow;
            3'b111:
                result=zero;
            default:
                result={data_len{1'b0}};
        endcase
    end

    adder_suber adder_suber_i(
                    .a        	( a                     ),
                    .b        	( b                     ),
                    .add_or_sub ( add_or_sub            ),
                    .result   	( result_adder_suber_i  ),
                    .carry    	(                       ),
                    .zero     	( zero                  ),
                    .overflow 	( overflow              )
                );

endmodule
