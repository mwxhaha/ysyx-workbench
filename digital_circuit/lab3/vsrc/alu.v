module alu
    #(
         parameter data_len=4
     )
     (
         input wire [data_len-1:0] a,b,
         input wire [2:0] func,
         output reg [data_len-1:0] result
     );
    
    wire [data_len-1:0] result1;
    reg addorsub;
    wire carry,zero,overflow;

    always@(*)
    begin
        case (func)
            3'b000: 
            begin
                addorsub=1'b0;
                result=result1;
            end
            3'b001:
            begin
                addorsub=1'b1;
                result=result1;
            end
            3'b010: 
            begin
                addorsub=1'b0;
                result=~a;
            end
            3'b011:
            begin
                addorsub=1'b0;
                result=a&b;
            end
            3'b100: 
            begin
                addorsub=1'b0;
                result=a|b;
            end
            3'b101:
            begin
                addorsub=1'b0;
                result=a^b;
            end
            3'b110: 
            begin
                addorsub=1'b1;
                result=result1[data_len-1]^overflow;
            end
            3'b111:
            begin
                addorsub=1'b1;
                result=zero;
            end
            default:
            begin
                addorsub=1'b0;
                result={data_len{1'b0}};
            end
        endcase
    end
    
    simple_alu simple_alu_i(
        .a        	( a         ),
        .b        	( b         ),
        .addorsub 	( addorsub  ),
        .result   	( result1   ),
        .carry    	( carry     ),
        .zero     	( zero      ),
        .overflow 	( overflow  )
    );
    

endmodule
