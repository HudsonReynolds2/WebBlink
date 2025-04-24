// Author: Xiang Jin
module pwm_sweep (
    input wire clk_30MHz,
    input wire reset,
    output reg servo_pwm
);

    // Constants
    localparam CLK_FREQ        = 30_000_000;
    localparam PWM_PERIOD_CLK  = 600_000;    // 20 ms
    localparam PULSE_0_DEG     = 30_000;     // 1 ms (0 degrees)
    localparam PULSE_180_DEG   = 60_000;     // 2 ms (180 degrees)

    reg [19:0] pwm_counter = 0;
    reg [19:0] pulse_width;

    always @(posedge clk_30MHz) begin
        // Set the pulse width based on reset
        if (reset)
            pulse_width <= PULSE_0_DEG;
        else
            pulse_width <= PULSE_180_DEG;

        // PWM output
        if (pwm_counter < pulse_width)
            servo_pwm <= 1;
        else
            servo_pwm <= 0;

        // PWM counter update
        if (pwm_counter >= PWM_PERIOD_CLK - 1)
            pwm_counter <= 0;
        else
            pwm_counter <= pwm_counter + 1;
    end

endmodule
