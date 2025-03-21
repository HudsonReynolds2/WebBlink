#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include "XPowersLib.h"
#include "pins_config.h"

#define FPGA_CS   PIN_FPGA_CS
#define HEARTBEAT_LED 46

XPowersAXP2101 PMU;
WebServer server(80);
QueueHandle_t spiQueue;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><body>
<h1>FPGA Blink Control</h1>
<form action="/start">Seconds:<input name="seconds" type="number" min="1"><input type="submit" value="Start"></form>
<form action="/stop"><input type="submit" value="Stop"></form>
</body></html>
)rawliteral";

void sendSPI(uint8_t v) {
  digitalWrite(FPGA_CS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  uint8_t resp = SPI.transfer(v);
  SPI.endTransaction();
  digitalWrite(FPGA_CS, HIGH);
  Serial.printf("→ Sent %d, FPGA reports %d left\n", v, resp);
}

void handleStart() {
  uint8_t secs = server.arg("seconds").toInt();
  xQueueOverwrite(spiQueue, &secs);
  server.sendHeader("Location","/",true);
  server.send(303);
}

void handleStop() {
  uint8_t zero = 0;
  xQueueOverwrite(spiQueue, &zero);
  server.sendHeader("Location","/",true);
  server.send(303);
}

void spiTask(void* _) {
  pinMode(FPGA_CS, OUTPUT);
  digitalWrite(FPGA_CS, HIGH);
  SPI.begin(PIN_FPGA_SCK, PIN_FPGA_D0, PIN_FPGA_D1);

  uint8_t cmd;
  while (true) {
    xQueueReceive(spiQueue, &cmd, portMAX_DELAY);

    // Send command (MSB=1)
    digitalWrite(FPGA_CS, LOW);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    uint8_t resp = SPI.transfer(cmd | 0x80);
    SPI.endTransaction();
    digitalWrite(FPGA_CS, HIGH);
    Serial.printf("→ Sent %d, FPGA reports %d left\n", cmd, resp);

    // Poll until countdown hits zero or new command arrives
    while (true) {
      // Preempt if new command
      if (xQueueReceive(spiQueue, &cmd, 0)) break;

      delay(1000);
      digitalWrite(FPGA_CS, LOW);
      SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
      resp = SPI.transfer(0x00);
      SPI.endTransaction();
      digitalWrite(FPGA_CS, HIGH);
      Serial.printf("⏳ Remaining: %d\n", resp);

      if (resp == 0) {
        Serial.println("✅ Done");
        break;
      }
    }
  }
}


void webTask(void*_) {
  pinMode(HEARTBEAT_LED, OUTPUT);
  WiFi.softAP("FPGA-Blink");
  server.on("/", [](){ server.send_P(200,"text/html",INDEX_HTML); });
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.begin();
  Serial.printf("AP IP = %s\n", WiFi.softAPIP().toString().c_str());
  while (true) {
    server.handleClient();
    digitalWrite(HEARTBEAT_LED, HIGH); 
    vTaskDelay(pdMS_TO_TICKS(250));
    digitalWrite(HEARTBEAT_LED, LOW);  
    vTaskDelay(pdMS_TO_TICKS(750));
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  if (!PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, PIN_IIC_SDA, PIN_IIC_SCL)) {
    Serial.println("PMU not found!");
    while(true) delay(100);
  }
  PMU.setDC4Voltage(1200);
  PMU.setALDO1Voltage(3300);
  PMU.setALDO2Voltage(3300);
  PMU.setALDO3Voltage(2500);
  PMU.setALDO4Voltage(1800);
  PMU.enableALDO1(); 
  PMU.enableALDO2(); 
  PMU.enableALDO3(); 
  PMU.enableALDO4();
  PMU.disableTSPinMeasure();
  
  delay(1000);
  Serial.println("PMU initialized.");

  spiQueue = xQueueCreate(1, sizeof(uint8_t));
  xTaskCreatePinnedToCore(spiTask, "SPI Task", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(webTask, "Web Task", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // nothing here — tasks handle everything
}
