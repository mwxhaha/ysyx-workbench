module mux21c(a,b,s,y);
  input   a,b,s;
  output reg  y;   // y在always块中被赋值，一定要声明为reg型的变量

  always @ (*)
    if(s==0)
      y = a;
    else
      y = b;
endmodule
