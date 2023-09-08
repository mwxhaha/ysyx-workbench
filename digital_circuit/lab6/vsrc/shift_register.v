module shift_register
    #(
         parameter data_len=4
     )
     (
         input wire clk,rst,q_serial_in,
         input wire [data_len-1:0] q_parallel_in,
         input wire [2:0] func,
         output reg [data_len-1:0] q
     );

    always@(posedge clk)
    begin
        if (rst==1'b1)
            q<={data_len{1'b0}};
        else
        begin
            case (func)
                3'b000:
                    q<={data_len{1'b0}};
                3'b001:
                    q<=q_parallel_in;
                3'b010:
                    q<=q>>1;
                3'b011:
                    q<=q<<1;
                3'b100:
                    q<={q[data_len-1],q[data_len-1:1]};
                3'b101:
                    q<={q_serial_in,q[data_len-1:1]};
                3'b110:
                    q<={q[0],q[data_len-1:1]};
                3'b111:
                    q<={q[data_len-2:0],q[data_len-1]};
                default:
                    q<={data_len{1'b0}};
            endcase
        end
    end

endmodule
