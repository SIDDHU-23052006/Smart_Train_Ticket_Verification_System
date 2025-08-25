#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define PIR_PIN 8

#define GREEN_LED 7
#define RED_LED 6
#define BUZZER_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);

int personCount = 0;
String scannedUIDs[100];
int totalUIDs = 0;

unsigned long lastPIRTime = 0;
bool lastPIRState = LOW;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(PIR_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("🚆 Smart Train Entry System Initialized (Jarvis Mode)");
}

void loop() {
  // RFID Ticket Scan
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uidString = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uidString += String(rfid.uid.uidByte[i], HEX);
    }
    uidString.toUpperCase();

    Serial.print("🎫 Scanned UID: ");
    Serial.println(uidString);

    if (!isAlreadyScanned(uidString)) {
      scannedUIDs[totalUIDs] = uidString;
      totalUIDs++;
      personCount++;

      Serial.print("✅ New Entry Registered. Count: ");
      Serial.println(personCount);

      flashLED(GREEN_LED);
    } else {
      Serial.println("❌ Duplicate Ticket Detected!");
      flashLED(RED_LED);
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(1000);
  }

  // PIR Exit or Unauthorized Detection
  bool pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH && lastPIRState == LOW && millis() - lastPIRTime > 2000) {
    lastPIRTime = millis();

    if (personCount > 0) {
      Serial.println("🚪 Person Exited. Removing first UID.");

      for (int i = 0; i < totalUIDs - 1; i++) {
        scannedUIDs[i] = scannedUIDs[i + 1];
      }

      totalUIDs--;
      personCount--;

      Serial.print("🧮 Updated Count: ");
      Serial.println(personCount);
    } else {
      Serial.println("🚨 Unauthorized Entry Detected! No ticket scanned.");
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(1000);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  lastPIRState = pirState;
}

// Check UID against stored list
bool isAlreadyScanned(String uid) {
  for (int i = 0; i < totalUIDs; i++) {
    if (scannedUIDs[i] == uid) {
      return true;
    }
  }
  return false;
}

// Blink feedback LED
void flashLED(int pin) {
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
}
