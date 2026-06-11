#define RXD2 17
#define TXD2 18

#include <Wire.h>
#include <PCF8575.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define PCF1_ADDR 0x24
#define PCF2_ADDR 0x20

PCF8575 pcf1(PCF1_ADDR);
PCF8575 pcf2(PCF2_ADDR);

/*
  ┌──────────────────────────────────────────┐
  │  PCF1 (0x24)                             │
  │  P0→c  P1→d  P2→e  P3→f                 │
  │  P4→g  P5→h  P6→i  P7→j                 │
  │  P8→b  P9→a                              │
  ├──────────────────────────────────────────┤
  │  PCF2 (0x20)                             │
  │  P0→m  P1→n  P2→o  P3→p                 │
  │  P4→q  P5→r  P6→s  P7→t                 │
  │  P8→l  P9→k                              │
  │  P11→u P12→v P13→w P14→x P15→y          │
  └──────────────────────────────────────────┘
*/

// ─── Struct for map entries ───────────────────────────────────────────────────
struct PinEntry {
  char    letter;
  uint8_t pin;
};

// ─── PCF1 map ─────────────────────────────────────────────────────────────────
const PinEntry pcf1Map[] = {
  {'k', P9}, {'l', P8},
  {'m', P0}, {'n', P1}, {'o', P2}, {'p', P3},
  {'q', P4}, {'r', P5}, {'s', P6}, {'t', P7}
};
const int pcf1MapSize = sizeof(pcf1Map) / sizeof(pcf1Map[0]);

// ─── PCF2 map ─────────────────────────────────────────────────────────────────
const PinEntry pcf2Map[] = {
  {'f', P9},  {'g', P8},
  {'h', P0},  {'i', P1},  {'j', P2},  {'a', P3},
  {'b', P4},  {'c', P5},  {'d', P6},  {'e', P7},
  {'u', P11}, {'v', P12}, {'w', P13}, {'x', P14}, {'y', P15}
};
const int pcf2MapSize = sizeof(pcf2Map) / sizeof(pcf2Map[0]);

// ─── Set one pin by letter ────────────────────────────────────────────────────
void setPin(char letter, bool state) {
  for (int i = 0; i < pcf1MapSize; i++) {
    if (pcf1Map[i].letter == letter) {
      pcf1.digitalWrite(pcf1Map[i].pin, state);
      Serial.printf("pcf1 (%c) → %s\n", letter, state ? "HIGH" : "LOW");
      return;
    }
  }
  for (int i = 0; i < pcf2MapSize; i++) {
    if (pcf2Map[i].letter == letter) {
      pcf2.digitalWrite(pcf2Map[i].pin, state);
      Serial.printf("pcf2 (%c) → %s\n", letter, state ? "HIGH" : "LOW");
      return;
    }
  }
  Serial.printf("'%c' — no pin mapping, ignored\n", letter);
}

// ─── All mapped pins HIGH ─────────────────────────────────────────────────────
void setAllHigh() {
  for (int i = 0; i < pcf1MapSize; i++) pcf1.digitalWrite(pcf1Map[i].pin, HIGH);
  for (int i = 0; i < pcf2MapSize; i++) pcf2.digitalWrite(pcf2Map[i].pin, HIGH);
  Serial.println("All used pins → HIGH");
}

// ─── All mapped pins LOW ──────────────────────────────────────────────────────
void setAllLow() {
  for (int i = 0; i < pcf1MapSize; i++) pcf1.digitalWrite(pcf1Map[i].pin, LOW);
  for (int i = 0; i < pcf2MapSize; i++) pcf2.digitalWrite(pcf2Map[i].pin, LOW);
  Serial.println("All used pins → LOW");
}

// ─── Handle incoming BT signal ────────────────────────────────────────────────
void handleSignal(char c) {
  if (c == '0') { setAllHigh(); return; }  // '0' → all HIGH
  if (c == '1') { setAllLow();  return; }  // '1' → all LOW

  if (c >= 'a' && c <= 'y') { setPin(c, LOW);              return; }  // lowercase → HIGH
  if (c >= 'A' && c <= 'Y') { setPin((c - 'A') + 'a', HIGH); return; }  // uppercase → LOW

  Serial.printf("Unknown signal: '%c' — ignored\n", c);
}

// ─── Setup ───────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("HC-05 Ready!");

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  pcf1.begin();
  pcf2.begin();

  for (int i = 0; i < pcf1MapSize; i++) pcf1.pinMode(pcf1Map[i].pin, OUTPUT);
  for (int i = 0; i < pcf2MapSize; i++) pcf2.pinMode(pcf2Map[i].pin, OUTPUT);

  setAllLow();
  Serial.println("PCF1 & PCF2 ready. All pins LOW.");
}

// ─── Loop ────────────────────────────────────────────────────────────────────
void loop() {
  while (Serial2.available()) {
    char received = Serial2.read();
    if (received == '\r' || received == '\n') continue;
    Serial.printf("BT received: '%c'\n", received);
    handleSignal(received);
  }
}