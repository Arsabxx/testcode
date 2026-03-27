// ==================== PWM Control Using Registers - Arduino Uno R4 WiFi ====================
// Mini Project: PWM output on D3 (IO D3) using GPT registers
// MCU: Renesas RA4M1
// Timer: GPT6 (D3 pin maps to GTIOC6B)
// Target: Maximum frequency + comparison of different settings

#include <stdint.h>

// GPT6 Register Base Address (from RA4M1 Hardware Manual)
#define GPT6_BASE       0x40074000UL

// Register definitions (key registers for PWM)
#define R_GPT6_GTWP     (*(volatile uint32_t*)(GPT6_BASE + 0x00))  // Write Protection Register
#define R_GPT6_GTSTR    (*(volatile uint32_t*)(GPT6_BASE + 0x04))  // Start Register
#define R_GPT6_GTSTP    (*(volatile uint32_t*)(GPT6_BASE + 0x08))  // Stop Register
#define R_GPT6_GTCR     (*(volatile uint32_t*)(GPT6_BASE + 0x10))  // Control Register
#define R_GPT6_GTPSC    (*(volatile uint32_t*)(GPT6_BASE + 0x1C))  // Prescaler (not always used directly)
#define R_GPT6_GTPR     (*(volatile uint32_t*)(GPT6_BASE + 0x28))  // Period Register (like ARR)
#define R_GPT6_GTCCR_B  (*(volatile uint32_t*)(GPT6_BASE + 0x2C))  // Compare Register B (Duty cycle for Channel B)
#define R_GPT6_GTIOR    (*(volatile uint32_t*)(GPT6_BASE + 0x38))  // I/O Control Register (output mode)
#define R_GPT6_GTBER    (*(volatile uint32_t*)(GPT6_BASE + 0x40))  // Buffer Enable
#define R_GPT6_GTUD     (*(volatile uint32_t*)(GPT6_BASE + 0x18))  // Count Direction

// Pin Function Select for D3 (P105 or correct port/pin - confirm with datasheet)
#define R_PFS_P105      (*(volatile uint32_t*)(0x40040000 + 0x0844 + (5*4)))  // Adjust port/pin if needed for your board

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("PWM Control using Registers - Arduino Uno R4 WiFi");
  Serial.println("Output: D3 (GPT6 Channel B)");

  // 1. Disable write protection
  R_GPT6_GTWP = 0xA5000000;   // Unlock protection (A5 magic value)

  // 2. Stop the timer first
  R_GPT6_GTSTP = 0x00000040;  // Stop GPT6 (bit for channel 6)

  // 3. Configure Pin D3 as GPT6 GTIOCB output (Alternate Function)
  // Typical PFS setting for GPT output on D3 (check exact value in RA4M1 datasheet - often 0x13 or 0x23)
  R_PFS_P105 = 0x00000000;           // Clear first
  R_PFS_P105 = 0x00000013;           // Set to GPT output (adjust if pin mapping differs)

  // 4. Basic GPT Configuration for Saw-wave PWM (single slope)
  R_GPT6_GTCR = 0x00000000;          // Clear control register
  R_GPT6_GTCR |= (0x1 << 16);        // Set PWM mode (saw wave)
  R_GPT6_GTUD = 0x00000001;          // Count up

  // === Change these values to test different frequencies and resolutions ===
  
  // Example 1: Maximum frequency (very low resolution)
  R_GPT6_GTPR    = 1;                // Period (ARR equivalent) = 1 → \~24 MHz theoretical
  R_GPT6_GTCCR_B = 1;                // Duty cycle ≈ 50%

  // Example 2: High frequency with better resolution (uncomment to test)
  // R_GPT6_GTPR    = 47;            // ≈ 1 MHz (48MHz / 48)
  // R_GPT6_GTCCR_B = 24;            // 50% duty

  // Example 3: Medium frequency (good for testing)
  // R_GPT6_GTPR    = 479;           // ≈ 100 kHz
  // R_GPT6_GTCCR_B = 240;           // 50% duty

  // 5. Configure output pin (GTIOCB - Channel B)
  R_GPT6_GTIOR = 0x00000000;         // Clear
  R_GPT6_GTIOR |= (0x1 << 0);        // Output enable for GTIOCB
  R_GPT6_GTIOR |= (0x6 << 8);        // PWM output mode (compare match)

  // Enable buffer if needed (optional for smoother updates)
  // R_GPT6_GTBER = 0x00000001;

  // 6. Start the timer
  R_GPT6_GTSTR = 0x00000040;         // Start GPT6

  Serial.println("PWM started on D3 - Check with oscilloscope!");
  Serial.println("Change GTPR and GTCCR_B values to compare different frequencies.");
}

void loop() {
  // Optional: Slowly change duty cycle for demonstration (for low frequency only)
  // For max frequency tests, keep it static or comment this out

  static uint32_t duty = 0;
  duty = (duty + 1) % (R_GPT6_GTPR + 1);   // Simple ramp
  R_GPT6_GTCCR_B = duty;

  Serial.print("Current Period (GTPR): ");
  Serial.print(R_GPT6_GTPR);
  Serial.print(" | Duty (GTCCR_B): ");
  Serial.println(R_GPT6_GTCCR_B);

  delay(1000);   // Slow update - remove or reduce for high frequency tests
}
