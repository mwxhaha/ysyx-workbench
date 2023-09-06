module digital_tube
    (
        input wire [3:0] data,
        output wire [7:0] HEX0,HEX1,HEX2,HEX3,HEX4,HEX5,HEX6,HEX7
    );

    bcd7seg seg7(data,HEX7);
    bcd7seg seg6(data,HEX6);
    bcd7seg seg5(data,HEX5);
    bcd7seg seg4(data,HEX4);
    bcd7seg seg3(data,HEX3);
    bcd7seg seg2(data,HEX2);
    bcd7seg seg1(data,HEX1);
    bcd7seg seg0(data,HEX0);

endmodule
