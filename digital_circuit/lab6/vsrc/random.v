module random
    #(
         parameter data_len=8
     )
     (
         input wire clk,rst,init,
         input wire [data_len-1:0] q_parallel_in,
         output reg [data_len-1:0] q
     );

    always@(posedge clk)
    begin
        if (rst==1'b1)
            q<={data_len{1'b0}};
        else if (init==1'b1)
            q<=q_parallel_in;
        else if (q=={data_len{1'b0}})
            q<={{(data_len-1){1'b0}},1'b1};
        else
            q<={q[4]^q[3]^q[2]^q[0],q[data_len-1:1]};
    end

endmodule
