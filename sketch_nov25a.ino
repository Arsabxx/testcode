void setup() {
  pinMode(8, OUTPUT);
  TCCR0B = 0;     // 停 Timer0
  TIMSK0 = 0;
  noInterrupts();
}

void loop() {
  while (1) {
    PORTB |= (1 << 0);
    delayMicroseconds(5);   // 調呢個 = HIGH 時間
    PORTB &= ~(1 << 0);
    delayMicroseconds(5);   // LOW 時間一樣 → 50%
  }
}