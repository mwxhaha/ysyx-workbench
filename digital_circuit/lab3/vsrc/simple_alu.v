module simple_alu
    #(
         parameter data_len=4
     )
     (
         input wire [data_len-1:0] a,b,
         input wire addorsub,
         output wire [data_len-1:0] result,
         output wire carry,zero,overflow
     );

    wire [data_len-1:0] b1;
    wire carry1;

    assign b1=b ^ {data_len{addorsub}};

    adder
        #(
            .data_len(data_len)
        )
        adder_i
        (
            .a    	( a        ),
            .b    	( b1       ),
            .cin  	( addorsub ),
            .s    	( result   ),
            .cout 	( carry1   )
        );

    assign carry=carry1^addorsub;

    assign zero=~(|result);

    assign overflow=(a[data_len-1] == b1[data_len-1]) && (result[data_len-1] != a[data_len-1]);

endmodule
