module barrel_shifter
    #(
         parameter data_len=8
     )
     (
         input wire [data_len-1:0] din,
         input wire [2:0] shamt,
         input wire left_or_right,algorism_or_logic,
         output reg [data_len-1:0] dout
     );

    wire sign_keep_or_quit;

    assign sign_keep_or_quit=algorism_or_logic?din[data_len-1]:1'b0;

    reg [data_len-1:0] d_1;
    reg [data_len-1:0] d_2;

    always@(*)
    begin
        case ({left_or_right,shamt[0]})
            2'b00:
                d_1=din;
            2'b01:
                d_1={sign_keep_or_quit,din[data_len-1:1]};
            2'b10:
                d_1=din;
            2'b11:
                d_1={din[data_len-2:0],1'b0};
            default:
                d_1=d_1;
        endcase
    end
    always@(*)
    begin
        case ({left_or_right,shamt[1]})
            2'b00:
                d_2=d_1;
            2'b01:
                d_2={{2{sign_keep_or_quit}},d_1[data_len-1:2]};
            2'b10:
                d_2=d_1;
            2'b11:
                d_2={d_1[data_len-3:0],{2{1'b0}}};
            default:
                d_2=d_2;
        endcase
    end
    always@(*)
    begin
        case ({left_or_right,shamt[2]})
            2'b00:
                dout=d_2;
            2'b01:
                dout={{4{sign_keep_or_quit}},d_2[data_len-1:4]};
            2'b10:
                dout=d_2;
            2'b11:
                dout={d_2[data_len-5:0],{4{1'b0}}};
            default:
                dout=dout;
        endcase
    end

endmodule
