#include <SPI.h>
#include <MFRC522.h>

// --- Pins (your wiring) ---
#define SS_PIN     10
#define RST_PIN     9
#define PIR_PIN     8
#define GREEN_LED   7
#define RED_LED     6
#define BUZZER      5

MFRC522 rfid(SS_PIN, RST_PIN);

// --- Authorized students (UID format with spaces) ---
String studentUIDs[] = {
  "13 C8 E7 FD",
  "A3 EA 21 14",
  "F3 AB 5D 28",
  "73 2C 36 F5"
};
String studentNames[] = {
  "Jean Kevin",
  "Priya Dharshan",
  "Siddharth B",
  "Vishnu P A"
};
const int totalStudents = 4;

// Format UID as "XX XX XX XX" in uppercase
String readUIDString() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();
  return uid;
}

String findStudent(const String& uid) {
  for (int i = 0; i < totalStudents; i++) {
    if (studentUIDs[i] == uid) return studentNames[i];
  }
  return "Unknown";
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(PIR_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Do NOT print any headers or debug lines (keeps CSV clean)
}

void loop() {
  // Wait for motion before reading
  if (digitalRead(PIR_PIN) == HIGH) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

      String uidString = readUIDString();
      String name = findStudent(uidString);

      // ---- CSV line: UID,Name ----
      Serial.print(uidString);
      Serial.print(",");
      Serial.println(name);  // NO timestamp here (PC will add it)

      // Feedback
      if (name != "Unknown") {
        digitalWrite(GREEN_LED, HIGH);
        tone(BUZZER, 1000, 200);
        delay(300);
        digitalWrite(GREEN_LED, LOW);
      } else {
        digitalWrite(RED_LED, HIGH);
        tone(BUZZER, 400, 800);
        delay(600);
        digitalWrite(RED_LED, LOW);
      }

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(500); // small cooldown
    }
  }
}
