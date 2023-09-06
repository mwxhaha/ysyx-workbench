module bcd7seg
    (
        input  [3:0] b,
        output reg [7:0] h
    );

    always @(*)
    begin
        case (b)
            4'b0000 :
                h = 8'b00000011;
            4'b0001 :
                h = 8'b10011111;
            4'b0010 :
                h = 8'b00100101;
            4'b0011 :
                h = 8'b00001101;
            4'b0100 :
                h = 8'b10011001;
            4'b0101 :
                h = 8'b01001001;
            4'b0110 :
                h = 8'b01000001;
            4'b0111 :
                h = 8'b00011111;
            4'b1000 :
                h = 8'b00000001;
            4'b1001 :
                h = 8'b00001001;
            default :
                h = 8'b11111111;
        endcase
    end
endmodule
