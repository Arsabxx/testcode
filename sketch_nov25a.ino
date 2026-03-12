void setup() {
  Serial.begin(9600);
  Serial.println("KY-039 Raw Waveform - 開始夾手指 30-60秒");
}

void loop() {
  int raw = analogRead(A0);
  Serial.println(raw);  // 只 print 數字
  delay(20);            // 50Hz 採樣
}