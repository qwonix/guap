// T-триггер с внешним асинхронным сбросом.
module t_flip_flop(input wire clk, reset, T, output reg Q);
    always @(posedge clk or posedge reset)
        if (reset) Q <= 1'b0;
        else Q <= Q ^ T;
endmodule
