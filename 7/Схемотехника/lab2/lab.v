module lr2(
    input wire x2, x1, x0,
    output wire y1, y0
);

    wire [7:0] q;

    wire group_n0;
    wire group_n1;
    wire group_n2;
    wire group_n3;

    decoder38 dec38(
        .x({x2, x1, x0}),
        .y(q)
    );

    assign group_n0 = q[2];

    assign group_n1 = q[0] & q[1] & q[6];

    assign group_n2 = q[4] & q[5] & q[7];

    assign group_n3 = q[3];

    encoder enc1(
        .n0(group_n0),
        .n1(group_n1),
        .n2(group_n2),
        .n3(group_n3),
        .n4(1'b1),
        .n5(1'b1),
        .n6(1'b1),
        .n7(1'b1),
        .EIN(1'b0),
        .A0N(y0),
        .A1N(y1)
    );

endmodule