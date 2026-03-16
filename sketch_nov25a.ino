void setup() {
  Serial.begin(115200);
  Serial.println("NTC Raw Value Test - Started");
  delay(2000);
}

void loop() {
  int raw = analogRead(A0);
  
  Serial.print("Raw ADC value: ");
  Serial.print(raw);
  Serial.print("   (Voltage approx: ");
  Serial.print(raw * 5.0 / 1023.0, 2);
  Serial.println(" V)");
  
  delay(500);  // 每半秒 print 一次
}