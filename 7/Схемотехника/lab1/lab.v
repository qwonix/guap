module lab1 (
    input  x2,
    input  x1,
    input  x0,
    output y1,
    output y0
);

assign y1 = (~x2 & ~x1) | (x1 & ~x0);

assign y0 = (~x2 & x1 & ~x0) |
            (x2 & ~x1) |
            (x2 & x0);

endmodule