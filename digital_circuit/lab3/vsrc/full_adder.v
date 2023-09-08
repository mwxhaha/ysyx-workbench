module full_adder
    (
        input wire a,b,cin,
        output wire s,cout
    );

    wire s_half_adder_i_1,c_half_adder_i_1,c_half_adder_i_2;

    half_adder half_adder_i_1
               (
                   .a 	( a                ),
                   .b 	( b                ),
                   .s 	( s_half_adder_i_1 ),
                   .c 	( c_half_adder_i_1 )
               );
    half_adder half_adder_i_2
               (
                   .a 	( cin               ),
                   .b 	( s_half_adder_i_1  ),
                   .s 	( s                 ),
                   .c 	( c_half_adder_i_2  )
               );
    assign cout = c_half_adder_i_1 | c_half_adder_i_2;

endmodule

