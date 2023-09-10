module ps2_keyboard(clk,rst,ps2_clk,ps2_data,data,
                        // ready,nextdata_n,overflow);
                        out);
    input clk,rst,ps2_clk,ps2_data;
    // input nextdata_n;
    wire nextdata_n;
    output [7:0] data;
    // output reg ready;
    reg ready;
    // output reg overflow;     // fifo overflow
    reg overflow;
    output reg [7:0] out;
    // internal signal, for test
    reg [9:0] buffer;        // ps2_data bits
    reg [7:0] fifo[7:0];     // data fifo
    reg [2:0] w_ptr,r_ptr;   // fifo write and read pointers
    reg [3:0] count;  // count ps2_data bits
    // detect falling edge of ps2_clk
    reg [2:0] ps2_clk_sync;

    always @(posedge clk)
    begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    always @(posedge clk)
    begin
        // if (rst==0)
        if (rst==1)
        begin // reset
            count <= 0;
            w_ptr <= 0;
            r_ptr <= 0;
            overflow <= 0;
            ready<= 0;
        end
        else
        begin
            if ( ready )
            begin // read to output next data
                if(nextdata_n == 1'b0) //read next data
                begin
                    r_ptr <= r_ptr + 3'b1;
                    if(w_ptr==(r_ptr+1'b1)) //empty
                        ready <= 1'b0;
                end
            end
            if (sampling)
            begin
                if (count == 4'd10)
                begin
                    if ((buffer[0] == 0) &&  // start bit
                            (ps2_data)       &&  // stop bit
                            (^buffer[9:1]))
                    begin      // odd  parity
                        fifo[w_ptr] <= buffer[8:1];  // kbd scan code
                        w_ptr <= w_ptr+3'b1;
                        ready <= 1'b1;
                        overflow <= overflow | (r_ptr == (w_ptr + 3'b1));
                    end
                    count <= 0;     // for next
                end
                else
                begin
                    buffer[count] <= ps2_data;  // store ps2_data
                    count <= count + 3'b1;
                end
            end
        end
    end
    assign data = fifo[r_ptr]; //always set output data

    reg nextdata_n_reg;

    always @(posedge clk)
    begin
        if(rst)
            nextdata_n_reg<=1'b1;
        else if (nextdata_n_reg==1'b0)
            nextdata_n_reg<=1'b1;
        else if (ready)
            nextdata_n_reg<=1'b0;
        else
            nextdata_n_reg<=nextdata_n_reg;
    end
    assign nextdata_n=nextdata_n_reg;

    reg shift_flag;

    always @(posedge clk)
    begin
        if(rst==1'b1)
            shift_flag <= 1'b0;
        else if (ready&&data==8'h12&&!break_flag)
            shift_flag<=1'b1;
        else if (ready&&data==8'h12&&break_flag)
            shift_flag<=1'b0;
        else
            shift_flag<=shift_flag;
    end

    reg break_flag;

    always @(posedge clk)
    begin
        if(rst==1'b1)
            break_flag <= 1'b0;
        else if (!nextdata_n&&data==8'hf0)
            break_flag<=1'b1;
        else if (!nextdata_n)
            break_flag<=1'b0;
        else
            break_flag<=break_flag;
    end

    always @(posedge clk)
    begin
        if(rst==1'b1)
        begin
            out <= 8'b0;
        end
        else if (ready&&break_flag)
            out<=8'b0;
        else if (ready&&shift_flag)
        begin
            case(data)
                8'h1c:
                    out<=8'h41;
                8'h1b:
                    out<=8'h53;
                8'h23:
                    out<=8'h44;
                default:
                    out<=out;
            endcase
        end
        else if (ready&&!shift_flag)
        begin
            case(data)
                8'h1c:
                    out<=8'h61;
                8'h1b:
                    out<=8'h73;
                8'h23:
                    out<=8'h64;
                default:
                    out<=out;
            endcase
        end
        else
            out<=out;
    end



endmodule
