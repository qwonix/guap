module mux8to1 (
    input  [7:0] d,
    input  [2:0] sel,
    output       y
);
    assign y = d[sel];
endmodule
