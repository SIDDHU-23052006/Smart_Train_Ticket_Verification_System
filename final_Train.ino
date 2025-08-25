#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SS_PIN 10
#define RST_PIN 9
#define PIR_PIN 8

#define GREEN_LED 7
#define RED_LED 6
#define BUZZER_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);

// OLED setup (I2C address 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

  // Initialize OLED with address 0x3D
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Smart Train Entry System"));
  display.println(F("Initializing..."));
  display.display();
  delay(1500);
  display.clearDisplay();

  Serial.println("🚆 Smart Train Entry System Initialized (Jarvis Mode)");
}

void loop() {
  // RFID Ticket Scan
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uidString = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      if(rfid.uid.uidByte[i] < 0x10) uidString += "0";
      uidString += String(rfid.uid.uidByte[i], HEX);
    }
    uidString.toUpperCase();

    Serial.print("🎫 Scanned UID: ");
    Serial.println(uidString);

    display.clearDisplay();
    display.setCursor(0,0);
    display.println(F("Ticket Scan:"));
    display.println(uidString);

    if (!isAlreadyScanned(uidString)) {
      scannedUIDs[totalUIDs] = uidString;
      totalUIDs++;
      personCount++;

      Serial.print("✅ New Entry Registered. Count: ");
      Serial.println(personCount);

      display.println(F("Entry Registered"));
      display.print(F("Count: "));
      display.println(personCount);

      flashLED(GREEN_LED);
    } else {
      Serial.println("❌ Duplicate Ticket Detected!");
      display.println(F("Duplicate Ticket!"));
      flashLED(RED_LED);
    }

    display.display();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(1000);
  }

  // PIR Exit or Unauthorized Detection
  bool pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH && lastPIRState == LOW && millis() - lastPIRTime > 2000) {
    lastPIRTime = millis();

    display.clearDisplay();
    display.setCursor(0,0);

    if (personCount > 0) {
      Serial.println("🚪 Person Exited. Removing first UID.");

      for (int i = 0; i < totalUIDs - 1; i++) {
        scannedUIDs[i] = scannedUIDs[i + 1];
      }

      totalUIDs--;
      personCount--;

      Serial.print("🧮 Updated Count: ");
      Serial.println(personCount);

      display.println(F("Person Exited"));
      display.print(F("Count: "));
      display.println(personCount);
    } else {
      Serial.println("🚨 Unauthorized Entry Detected! No ticket scanned.");
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);

      display.println(F("Unauthorized Entry!"));
      delay(1000);

      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }
    display.display();
  }

  lastPIRState = pirState;

  // Continuously show current count on OLED bottom
  display.setTextSize(1);
  display.setCursor(0, SCREEN_HEIGHT - 10);
  display.print(F("Current Count: "));
  display.println(personCount);
  display.display();
  delay(200);
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
