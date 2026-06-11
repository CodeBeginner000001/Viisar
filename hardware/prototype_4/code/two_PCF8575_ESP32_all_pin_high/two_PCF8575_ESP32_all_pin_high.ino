// WIRING OF ESP32 S3 N16R8 to PCF8575
/*

* PCF8575 ---->>> ESP32 S3 N16R8
- SDA ---->>> 8
- SCL ---->>> 9
- VCC ---->>> 3.3v
- GND ---->>> GND
- VDD ---->>> 3.3v

* PCF8575 ---->>> ESP32
- SDA ---->>> SDA pin of esp32
- SCL ---->>> SCL pin of esp32
- VCC ---->>> 3.3v
- GND ---->>> GND
- VDD ---->>> 3.3v

*/
#include <Arduino.h>
#include <Wire.h>
#include <PCF8575.h>

#define SDA_PIN 8
#define SCL_PIN 9

#define PCF1_ADDR 0x24
#define PCF2_ADDR 0x20  // change if needed

PCF8575 pcf1(PCF1_ADDR);
PCF8575 pcf2(PCF2_ADDR);

void setAllPinHigh(PCF8575 &pcfModule) {
  for (int pin = P0; pin <= P15; pin++) {
    pcfModule.digitalWrite(pin, HIGH);
  }
}

void setAllPinLow(PCF8575 &pcfModule) {
  for (int pin = P0; pin <= P15; pin++) {
    pcfModule.digitalWrite(pin, LOW);
  }
}

void toggleAllPin(PCF8575 &pcfModule) {
  for (int pin = P0; pin <= P15; pin++) {
    pcfModule.digitalWrite(pin, HIGH);
    delay(800);
    pcfModule.digitalWrite(pin, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  pcf1.begin();
  pcf2.begin();

  // Set all pins as OUTPUT once
  for (int pin = P0; pin <= P15; pin++) {
    pcf1.pinMode(pin, OUTPUT);
    pcf2.pinMode(pin, OUTPUT);
  }

  Serial.println("PCF8575 ready. Setting all pins HIGH in loop...");
}

void loop() {
  // toggleAllPin(pcf1);
  setAllPinLow(pcf1);
  setAllPinLow(pcf2);
  delay(5000);
  setAllPinHigh(pcf1);
  setAllPinHigh(pcf2);
  delay(5000);

}