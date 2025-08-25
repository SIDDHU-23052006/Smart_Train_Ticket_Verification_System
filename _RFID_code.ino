#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

int personCount = 0;

// Store known UIDs
String scannedUIDs[100];  // assuming max 100 unique people
int totalUIDs = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin(); 
  rfid.PCD_Init(); 
  Serial.println("RFID Scanner Initialized.");
}

void loop() {
  // Look for new RFID card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Read UID
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }

  uidString.toUpperCase();
  Serial.print("Scanned UID: ");
  Serial.println(uidString);

  // Check if already counted
  if (!isAlreadyScanned(uidString)) {
    scannedUIDs[totalUIDs] = uidString;
    totalUIDs++;
    personCount++;

    Serial.print("New Person Detected. Total Count: ");
    Serial.println(personCount);
  } else {
    Serial.println("Already Scanned. Count not increased.");
  }

  rfid.PICC_HaltA(); // Halt card
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD
  delay(1000);
}

bool isAlreadyScanned(String uid) {
  for (int i = 0; i < totalUIDs; i++) {
    if (scannedUIDs[i] == uid) {
      return true;
    }
  }
  return false;
}
