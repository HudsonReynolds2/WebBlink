// Author: Xiang Jin
module clock_divider (
    input wire clk_30mhz,
    input wire reset,
    output reg clk_1hz
);

    // Number of cycles for half a second at 30 MHz
    localparam integer HALF_PERIOD_COUNT = 40_000 / 2;  // 15,000,000

    reg [24:0] counter;  // Enough bits to count up to 15 million

    always @(posedge clk_30mhz or posedge reset) begin
        if (reset) begin
            counter <= 0;
            clk_1hz <= 0;
        end else begin
            if (counter == HALF_PERIOD_COUNT - 1) begin
                counter <= 0;
                clk_1hz <= ~clk_1hz;
            end else begin
                counter <= counter + 1;
            end
        end
    end

endmodule
