#include <FspTimer.h>           // Required library for hardware timers on UNO R4

FspTimer myTimer;               // Create timer object

void setup() {
  pinMode(8, OUTPUT);           // Pin 8 – connected to GPT channel (GTIOC7A on RA4M1)

  // Initialize GPT timer for periodic PWM output on pin 8
  bool success = myTimer.begin(
    TIMER_MODE_PERIODIC,        // Continuous square wave mode
    TIMER_SOURCE_DIV1,          // No prescaler – fastest clock
    10000.0f,                   // Desired frequency in Hz (10 kHz here – change as needed)
    50.0f,                      // Duty cycle in % (50% = perfect square wave)
    TIMER_UNIT_GPT,             // Use General PWM Timer (GPT)
    nullptr                     // No callback function needed for pure PWM
  );

  if (!success) {
    // Timer failed to start (possible wrong pin/channel or conflict)
    while (1);                  // Halt here for debugging
  }

  myTimer.start();              // Start hardware PWM – wave is generated automatically
}

void loop() {
  // Nothing needed here – hardware timer controls pin 8 directly
  // You can change frequency/duty dynamically if desired:
  // myTimer.set_frequency(20000.0f);     // Change to 20 kHz
  // myTimer.set_duty_cycle(75.0f);       // Change to 75% duty
}