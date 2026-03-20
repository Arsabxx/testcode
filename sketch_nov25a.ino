
void setup() {
  pinMode(8, OUTPUT);   // Set D8 as output
  cli();                // Disable interrupts for maximum stability
}

void loop() {
  while (1) {           // Tight infinite loop for highest speed
    PORTB |= (1 << 0);   // Set D8 HIGH (sbi instruction, 2 cycles)
    PORTB &= ~(1 << 0);  // Set D8 LOW  (cbi instruction, 2 cycles)
    // rjmp back to loop adds ~2 cycles → total 6 cycles per full cycle
  }
}