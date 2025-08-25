void setup() {
  Serial.begin(9600);
  Serial.println("Hello from ESP32-CAM!");
}

void loop() {
  delay(2000);
  Serial.println("Running...");
}
