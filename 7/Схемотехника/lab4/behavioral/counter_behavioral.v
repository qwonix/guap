// Lab 4, Variant 11: synchronous modulo-15 up counter (Quartus II 13.1).
module counter_behavioral(
    input wire clk,
    input wire enable,
    input wire reset,
    output reg [3:0] counter,
    output reg cout
);
    localparam [3:0] LAST = 4'd14;

    // The terminal state is cleared synchronously on the next rising edge.
    // This branch precedes enable, matching the functional specification.
    always @(posedge clk or posedge reset) begin
        if (reset)
            counter <= 4'd0;
        else if (counter == LAST)
            counter <= 4'd0;
        else if (enable)
            counter <= counter + 1'b1;
    end

    // Overflow/cascade pulse is sampled on the falling edge of clk.
    always @(negedge clk or posedge reset) begin
        if (reset)
            cout <= 1'b0;
        else
            cout <= (counter == LAST);
    end
endmodule
