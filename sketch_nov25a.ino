// ==================== PWM Control Using Registers - Arduino Uno R4 WiFi ====================
// Output: D3 (P105 - GTIOC1A of GPT1)
// Timer clock: 48 MHz (PCLKD)
// Theoretical max frequency: \~24 MHz (but practical limit \~5-10 MHz with good waveform)

#include <stdint.h>

// GPT1 Register Base (RA4M1 GPT1)
#define GPT1_BASE       0x40070000UL

#define R_GPT1_GTWP     (*(volatile uint32_t*)(GPT1_BASE + 0x000))  // Write Protection
#define R_GPT1_GTSTR    (*(volatile uint32_t*)(GPT1_BASE + 0x004))  // Start
#define R_GPT1_GTSTP    (*(volatile uint32_t*)(GPT1_BASE + 0x008))  // Stop
#define R_GPT1_GTCR     (*(volatile uint32_t*)(GPT1_BASE + 0x010))  // Control
#define R_GPT1_GTPSC    (*(volatile uint32_t*)(GPT1_BASE + 0x01C))  // Prescaler (usually not used)
#define R_GPT1_GTPR     (*(volatile uint32_t*)(GPT1_BASE + 0x028))  // Period Register (ARR)
#define R_GPT1_GTCCR_A  (*(volatile uint32_t*)(GPT1_BASE + 0x028 + 0x04)) // Compare Register A (Duty for Channel A)
#define R_GPT1_GTIOR    (*(volatile uint32_t*)(GPT1_BASE + 0x038))  // I/O Control Register
#define R_GPT1_GTBER    (*(volatile uint32_t*)(GPT1_BASE + 0x040))  // Buffer Enable

// PFS (Pin Function Select) for D3 = P105
#define R_PFS_BASE      0x40040000UL
#define R_PFS_P105      (*(volatile uint32_t*)(R_PFS_BASE + 0x0844 + (5*4)))  // Port 1, Pin 5? Wait, correct for P105

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("PWM on D3 (GPT1 GTIOC1A) - Direct Register Control");

  // 1. Unlock write protection
  R_GPT1_GTWP = 0xA5000000UL;   // Magic value to unlock

  // 2. Stop timer
  R_GPT1_GTSTP = 0x00000002UL;  // Bit 1 for GPT1

  // 3. Configure Pin D3 (P105) as GTIOC1A output
  // PFS setting for GPT output (PSL value for GTIOC1A on P105 is usually 0x13 or 0x0B - test both if needed)
  R_PFS_P105 = 0x00000000;      // Clear
  R_PFS_P105 = 0x00000013;      // Set to GPT alternate function (common value)

  // 4. Basic GPT1 configuration for PWM (Saw-wave, count up)
  R_GPT1_GTCR = 0x00000000;
  R_GPT1_GTCR |= (1UL << 16);   // PWM mode (saw wave)
  R_GPT1_GTCR |= (1UL << 0);    // Count up (optional)

  // === Test different frequencies here (change these two lines) ===
  
  // MAX frequency (low resolution) - for your "max freq" test
  R_GPT1_GTPR    = 1;           // Period = 1 → \~24 MHz theoretical
  R_GPT1_GTCCR_A = 1;           // Duty ≈ 50%

  // Better high frequency example (\~1 MHz)
  // R_GPT1_GTPR    = 47;       // 48MHz / 48 ≈ 1 MHz
  // R_GPT1_GTCCR_A = 24;       // 50% duty

  // Medium frequency (good waveform, recommended for report)
  // R_GPT1_GTPR    = 479;      // ≈ 100 kHz
  // R_GPT1_GTCCR_A = 240;      // 50% duty

  // 5. Configure GTIOC1A output (Channel A)
  R_GPT1_GTIOR = 0x00000000;
  R_GPT1_GTIOR |= (1UL << 0);   // GTIOCA output enable
  R_GPT1_GTIOR |= (0x6UL << 8); // PWM output mode (toggle on compare match for PWM)

  // Optional: Enable buffer for smoother operation
  // R_GPT1_GTBER = 0x00000001;

  // 6. Start the timer
  R_GPT1_GTSTR = 0x00000002UL;  // Start GPT1

  Serial.println("PWM started on D3!");
  Serial.println("Connect oscilloscope to D3 and check waveform.");
}

void loop() {
  // For max frequency test, leave empty or slow update
  // You can add Serial output here to monitor current settings
  delay(2000);
  Serial.print("Current Period (GTPR): ");
  Serial.println(R_GPT1_GTPR);
}
