#include <Arduino.h>
#include <Wire.h>
#include <PCF8575.h>

#define SDA_PIN 8
#define SCL_PIN 9

PCF8575 pcf8575(0x24);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  pcf8575.begin();

  // Set all pins as OUTPUT and start LOW
  for (int pin = P11; pin <= P15; pin++) {
    pcf8575.pinMode(pin, OUTPUT);
    pcf8575.digitalWrite(pin, LOW);
  }

  Serial.println("PCF8575: Turning ON and OFF each pin one-by-one...");
}

void loop() {
  for (int pin = P11; pin <= P15; pin++) {
    Serial.print("Pin ON: P"); Serial.println(pin);
    pcf8575.digitalWrite(pin, HIGH);
    delay(800);

    Serial.print("Pin OFF: P"); Serial.println(pin);
    pcf8575.digitalWrite(pin, LOW);
    delay(800);
  }
  // pcf8575.digitalWrite(P11, HIGH);
  // pcf8575.digitalWrite(P12, HIGH);
  // pcf8575.digitalWrite(P13, HIGH);
  // pcf8575.digitalWrite(P14, HIGH);
  // pcf8575.digitalWrite(P15, HIGH);
}