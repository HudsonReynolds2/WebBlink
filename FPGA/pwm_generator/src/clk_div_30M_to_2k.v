// Author: Xiang Jin
module clk_div_30M_to_2k (
    input wire clk_30MHz,
    input wire reset,
    output reg clk_2kHz
);

    // Half-period count for 2kHz square wave
    localparam integer HALF_PERIOD = 7500;

    reg [13:0] counter = 0;  // 14 bits enough for counting to 7500

    always @(posedge clk_30MHz or posedge reset) begin
        if (reset) begin
            counter <= 0;
            clk_2kHz <= 0;
        end else begin
            if (counter == HALF_PERIOD - 1) begin
                counter <= 0;
                clk_2kHz <= ~clk_2kHz;
            end else begin
                counter <= counter + 1;
            end
        end
    end

endmodule
