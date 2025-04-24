// Author: Xiang Jin
module led_blinker(
   input clk,
   input reset,
   output reg led
);


wire divided_clk;

clock_divider(clk, reset, divided_clk);

always @(posedge divided_clk or posedge reset) begin
    if (reset)
        led <= 0;
    else 
        led <= !led;
end

endmodule
