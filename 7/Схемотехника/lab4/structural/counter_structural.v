// Lab 4, Variant 11: modulo-15 counter built from T flip-flops.
module counter_structural(
    input wire clk,
    input wire enable,
    input wire reset,
    output wire [3:0] counter,
    output reg cout
);
    wire [3:0] Q;
    wire [3:0] T;
    wire terminal;

    // State 1110 must go directly to 0000; terminal is used only in
    // synchronous T-input equations, never in the asynchronous reset path.
    assign terminal = Q[3] & Q[2] & Q[1] & ~Q[0];
    assign T[0] = enable & ~terminal;
    assign T[1] = (enable & Q[0]) | terminal;
    assign T[2] = (enable & Q[0] & Q[1]) | terminal;
    assign T[3] = (enable & Q[0] & Q[1] & Q[2]) | terminal;

    t_flip_flop TQ0(.clk(clk), .reset(reset), .T(T[0]), .Q(Q[0]));
    t_flip_flop TQ1(.clk(clk), .reset(reset), .T(T[1]), .Q(Q[1]));
    t_flip_flop TQ2(.clk(clk), .reset(reset), .T(T[2]), .Q(Q[2]));
    t_flip_flop TQ3(.clk(clk), .reset(reset), .T(T[3]), .Q(Q[3]));

    assign counter = Q;

    always @(negedge clk or posedge reset) begin
        if (reset)
            cout <= 1'b0;
        else
            cout <= terminal;
    end
endmodule
