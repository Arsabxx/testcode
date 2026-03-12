void setup() {
  Serial.begin(9600);
  Serial.println("KY-039 Raw + BPM Test - 夾手指遮光");
}

static int lastRaw = 0;
static unsigned long lastBeat = 0;
static int bpm = 0;

void loop() {
  int raw = analogRead(A0);

  if (raw > lastRaw + 25 && millis() - lastBeat > 200) {
    unsigned long interval = millis() - lastBeat;
    if (interval > 200 && interval < 5000) {
      bpm = 60000 / interval;
      lastBeat = millis();
      Serial.print(" [Beat!] Diff: "); Serial.print(raw - lastRaw);
      Serial.print(" BPM: "); Serial.println(bpm);
    }
  }

  lastRaw = raw;

  Serial.print("Raw: "); Serial.print(raw);
  Serial.print(" BPM: "); Serial.println(bpm);

  delay(20);  // 快讀
}