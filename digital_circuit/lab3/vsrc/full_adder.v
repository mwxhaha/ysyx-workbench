module full_adder
    (
        input wire a,b,cin,
        output wire s,cout
    );

    wire s1,c1,c2;

    half_adder half_adder_i_1
               (
                   .a 	( a  ),
                   .b 	( b  ),
                   .s 	( s1 ),
                   .c 	( c1 )
               );
    half_adder half_adder_i_2
               (
                   .a 	( cin ),
                   .b 	( s1  ),
                   .s 	( s   ),
                   .c 	( c2  )
               );
    assign cout=c1|c2;

endmodule

