module encoder(
    input wire n0, n1, n2, n3,
    input wire n4, n5, n6, n7,
    input wire EIN,
    output wire EON, GSN,
    output wire A0N, A1N, A2N
);

    \74148 adapted (
        .\0N(n0),
        .\1N(n1),
        .\2N(n2),
        .\3N(n3),
        .\4N(n4),
        .\5N(n5),
        .\6N(n6),
        .\7N(n7),
        .EIN(EIN),
        .EON(EON),
        .GSN(GSN),
        .A0N(A0N),
        .A1N(A1N),
        .A2N(A2N)
    );

endmodule