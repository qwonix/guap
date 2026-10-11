module code_converter (
    input  x2,
    input  x1,
    input  x0,
    output y1,
    output y0
);
    wire [2:0] sel;
    assign sel = {x2, x1, x0};

    mux8to1 mux_y1 (.d(8'b01000111), .sel(sel), .y(y1));
    mux8to1 mux_y0 (.d(8'b10110100), .sel(sel), .y(y0));
endmodule
