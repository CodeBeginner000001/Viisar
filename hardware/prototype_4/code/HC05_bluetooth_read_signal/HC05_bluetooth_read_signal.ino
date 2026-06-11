#define RXD2 17
#define TXD2 18

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("HC-05 Ready! Waiting for data...");
}

void loop() {
  if (Serial2.available()) {
    char received = Serial2.read();  // ✅ char not string
    if (received != '\r' && received != '\n') {
      Serial.print("Received: ");
      Serial.println(received);
    }
  }
}