#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create display object for I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  
  // Start OLED (try 0x3C first)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 0x3C failed, trying 0x3D..."));
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
      Serial.println(F("OLED init failed! Check wiring."));
      while (true); // Stop here if no display found
    }
  }

  // Clear the buffer
  display.clearDisplay();

  // Set text properties
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Print initial message
  display.setCursor(0, 0);
  display.println("Smart Train Ticket   Verification System");
  display.display();
  delay(2000);
}

void loop() {
  // nothing to do here
}
