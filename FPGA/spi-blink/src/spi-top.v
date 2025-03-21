`timescale 1ns/1ps
module spi_top(
    input        cs,
    input        sck,
    input        MOSI,
    output       MISO,
    output reg   blink_pin
);

wire oscout, clk30;
wire [7:0] rxd_out;
wire       rxd_flag;
reg  [6:0] count;
reg [24:0] sec_cnt;
wire [7:0] txd_data = {1'b0, count};

// Clock generation
Gowin_OSC osc(.oscout(oscout), .oscen(1'b1));
Gowin_PLLVR pll(.clkout(clk30), .clkin(oscout));

// SPI slave IP
spi_slaver slave (
    .clk(clk30), 
    .rst(1'b1),
    .cs(cs), 
    .sck(sck), 
    .MOSI(MOSI), 
    .MISO(MISO),
    .rxd_out(rxd_out), 
    .txd_data(txd_data),
    .rxd_flag(rxd_flag)
);

// Countdown + blink logic
always @(posedge clk30) begin
    if (rxd_flag && rxd_out[7]) begin
        count     <= rxd_out[6:0];
        blink_pin <= 1'b0;
        sec_cnt   <= 25'd0;
    end
    else if (count != 0) begin
        if (sec_cnt == 25_000_000) begin
            sec_cnt   <= 25'd0;
            blink_pin <= ~blink_pin;
            if (!blink_pin)
                count <= count - 1;
        end else begin
            sec_cnt <= sec_cnt + 1;
        end
    end else begin
        blink_pin <= 1'b0;  // LED OFF when timer done
    end
end

endmodule
