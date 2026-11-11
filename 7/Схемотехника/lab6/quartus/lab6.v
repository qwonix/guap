module lab6 (
    input wire clk,
    output reg [3:0] q = 4'd0
);
    reg descending = 1'b0;

    always @(posedge clk) begin
        if (!descending) begin
            if (q == 4'd15) begin
                q <= 4'd14;
                descending <= 1'b1;
            end else begin
                q <= q + 4'd1;
            end
        end else begin
            if (q == 4'd0) begin
                q <= 4'd1;
                descending <= 1'b0;
            end else begin
                q <= q - 4'd1;
            end
        end
    end
endmodule
