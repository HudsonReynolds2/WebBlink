<h1 align = "center"> WebBlink </h1>

# An extension of the LED Blink Example for LilyGO T-FPGA
## Description
The onboard ESP32S3 creates a wepbage where a user can enter the number of seconds to blink Bank2 IOR15A.
The webserver runs on one core of the ESP32 with a heartbeat signal that flashes onboard LED GPIO46, while the other core communicates with SPI to the FPGA. The FPGA gets the time to blink, then starts a timer, which ends when the timer expires or it gets sent a stop command. The FPGA sends back the remaining time over SPI to the ESP32 which prints it out in the serial monitor so the user can see what the FPGA is doing. This example can be extended to other simple tasks to utilize both chips onboard the LilyGO T-FPGA. Notably the hard core CPU in the FPGA is still under utilized.
This code was extended from the LilyGO blink example.

## Quick Start
Follow the Arduino directions at the start of the README.md at this repo to upload the code: https://github.com/Xinyuan-LilyGO/T-FPGA/tree/main

Make sure to put an LED into Bank2 that is protected by a resistor. IOR15A will have around 2.442 volts, 8mA.

The webpage is accessible via these credentials:
```
WiFi SSID: FPGA-Blink
```
Then open a browser and go to ***192.168.4.1***

You should see this page load:

<img src="webpage.jpg" width="500" height="1002">
