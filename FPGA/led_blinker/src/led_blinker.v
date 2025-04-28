// Author: Xiang Jin
module led_blinker(
   input clk,
   input reset,
   output reg led
);


wire divided_clk;

clock_divider(clk, divided_clk);

always @(posedge divided_clk) begin
    if (!reset)
        led <= 0;
    else 
        led <= !led;
end

endmodule
