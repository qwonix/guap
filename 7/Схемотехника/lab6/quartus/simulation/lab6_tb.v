`timescale 1ns/1ps
module lab6_tb;
    reg clk = 0;
    wire [3:0] q;
    integer i, expected;
    integer errors = 0;
    integer glitches = 0;
    realtime edge_time;
    lab6 dut (.clk(clk), .q(q));
    always #50 clk = ~clk;
    always @(posedge clk) edge_time = $realtime;
    always @(q) begin
`ifndef TIMING
        if ($time > 0 && ($time % 100) != 50) glitches = glitches + 1;
`else
        if ($time > 0 && $time < 400)
            $display("OUTPUT_CHANGE time=%0.3f ns q=%d delay=%0.3f ns", $realtime, q, $realtime-edge_time);
`endif
    end
    initial begin
        #30;
        if (q !== 4'd0) errors = errors + 1;
        for (i = 1; i <= 90; i = i + 1) begin
            @(posedge clk);
`ifdef TIMING
            #30;
`else
            #1;
`endif
            expected = i % 30;
            if (expected > 15) expected = 30 - expected;
            if (q !== expected) begin
                $display("ERROR tick=%0d actual=%0d expected=%0d", i, q, expected);
                errors = errors + 1;
            end
        end
        $display("RESULT ticks=%0d errors=%0d unexpected_changes=%0d", i-1, errors, glitches);
        $stop;
    end
endmodule
