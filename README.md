<h1 align="center">LilyGO T-FPGA Custom Examples</h1>

# Overview

This repository contains a collection of verified and functional examples for the LilyGO T-FPGA development board. Each example demonstrates different functionalities, combining the capabilities of the onboard ESP32-S3 and the FPGA. These examples serve as practical references for developers exploring the potential of the T-FPGA platform. *Notably, none of these examples make use of the hard core ARM Cortex M3 inside the FPGA. If you want to make use of it, GOWIN EDA has some resources. Please leave a comment about your experiences with it on this repo so we can all learn!*

The original example repo from LilyGo can be found here: [LilyGO T-FPGA GitHub Repository](https://github.com/Xinyuan-LilyGO/T-FPGA/tree/main). There is a `schematic/` folder which shows you how to map from the constraint file pin assignments for the FPGA to the actual pin headers on the shield. So pin 22 becomes IO22A, others map less linearly. 

## Contents

- **Web Blink**: Control an FPGA-connected LED via a web interface hosted by the ESP32.
- **PWM Generator**: Generate PWM signals using the FPGA, with control inputs from the onboard button.
- **LED Blink**: A standalone FPGA example that blinks an onboard LED without ESP32 intervention.

---

## Web Blink

### Description
Code Author: Hudson Reynolds

The ESP32-S3 hosts a webpage allowing users to input a duration for blinking an LED connected to FPGA Bank2 **IOR15A**. The ESP32 manages the webserver and communicates with the FPGA via SPI. The FPGA handles the timing and blinking logic, sending status updates back to the ESP32 for serial monitoring. The ESP32 blinks its own heartbeat LED to show it is running.

### Quick Start

1. Connect both USB-C ports of the T-FPGA to your computer.
2. Upload the ESP32 code using the Arduino IDE. Press the **RESET** button onboard the T-FPGA to configure the PMU.
3. Launch the GOWIN FPGA Designer and open the FPGA project `spi-blink` from `FPGA/spi-blink/`.
4. Run Synthesis and ensure there are no errors.
5. Use the GOWIN Programmer to upload the bitstream to the FPGA.
6. Ensure the programmer cable connects correctly; if not, consider using Zadig to adjust USB drivers which is briefly discussed on the main LilyGO repo. 
7. Place an LED (with a current-limiting resistor) into Bank2 **IOR15A**.
8. Connect to the ESP32's Wi-Fi network:

   ```
   SSID: FPGA-Blink
   ```

9. Open a browser and navigate to `192.168.4.1` to access the control webpage.

![Web Interface](webpage.jpg)

---

## PWM Generator

### Description
Code Author: Xiang Jin

This example demonstrates PWM signal generation using the FPGA. The PWM signal is output on the "servo pin" from the constraint file (22), which on the board is pin **IOB22A**. The duty cycle is either 10% (default) or 5% (holding the onboard button **IOT13B**). Holding spins a servo towards 0°, otherwise its spins towards 180°.

The ESP32 is used solely to configure the Power Management Unit (PMU) to supply the necessary voltage to the FPGA's Bank2. No further interaction with the ESP32 is required for this example.

### Quick Start

1. Connect both USB-C ports of the T-FPGA to your computer.
2. Upload the ESP32 code using the Arduino IDE. This can be created by cutting out any code you don't want from `ESP32/web_blink/web_blink.ino`. Then press the **RESET** button onboard the T-FPGA.
3. Launch the GOWIN FPGA Designer and open the `pwm_generator` project from `FPGA/pwm_generator`.
4. Run Synthesis and ensure there are no errors.
5. Use the GOWIN Programmer to upload the bitstream to the FPGA.
6. Connect your PWM-controlled device (e.g., servo) to **IOB22A**.
7. Hold the onboard button **IOT13B** to spin the servo towards 0°, let go and it will spin towards 180°. Duty Cycle: Button Held: 5%, Unheld: 10%.

---

## LED Blink

### Description
Code Author: Xiang Jin

A simple FPGA-only example that blinks the onboard LED connected to **IOB5A** without any ESP32 involvement. Note: After programming, the ESP32 may need to be power-cycled or disconnected for the LED to start blinking due to PMU-related power bank activation.

### Quick Start

1. Connect both USB-C ports of the T-FPGA to your computer.
2. Upload the ESP32 code using the Arduino IDE. This can be created by cutting out any code you don't want from `ESP32/web_blink/web_blink.ino`. Then press the **RESET** button onboard the T-FPGA.
3. Launch the GOWIN FPGA Designer and open the `led_blinker` project from `FPGA/led_blinker`.
4. Run Synthesis and ensure there are no errors.
5. Use the GOWIN Programmer to upload the bitstream to the FPGA.
6. If the LED doesn't start blinking immediately, unplug and replug the ESP32 to reset the power banks.

---

## Additional Notes

- Ensure all connections are secure before uploading code or bitstreams.
- Refer to the [LilyGO T-FPGA GitHub Repository](https://github.com/Xinyuan-LilyGO/T-FPGA/tree/main) for detailed setup instructions and troubleshooting.

---
