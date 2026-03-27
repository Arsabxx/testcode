// Simple Direct Register PWM on D3 (Uno R4 WiFi)
// Pin: D3 = P105 = GTIOC1A of GPT1
// Timer clock \~48 MHz

#include <stdint.h>

// GPT1 Base Address
#define GPT1_BASE  0x40070000UL

#define GTWP   (*(volatile uint32_t*)(GPT1_BASE + 0x000))  // Write Protection
#define GTSTR  (*(volatile uint32_t*)(GPT1_BASE + 0x004))  // Start
#define GTSTP  (*(volatile uint32_t*)(GPT1_BASE + 0x008))  // Stop
#define GTCR   (*(volatile uint32_t*)(GPT1_BASE + 0x010))  // Control
#define GTPR   (*(volatile uint32_t*)(GPT1_BASE + 0x028))  // Period (ARR)
#define GTCCR0 (*(volatile uint32_t*)(GPT1_BASE + 0x02C))  // Compare Register A (Duty)
#define GTIOR  (*(volatile uint32_t*)(GPT1_BASE + 0x038))  // I/O Control

// PFS for P105 (D3)
#define PFS_P105 (*(volatile uint32_t*)(0x40040800 + 0xA8))  // Correct address for P105

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== PWM Register Test on D3 (R4 WiFi) ===");

  // 1. Unlock write protection
  GTWP = 0xA5000000UL;

  // 2. Stop GPT1
  GTSTP = 0x00000002UL;   // Bit for GPT1

  // 3. Configure Pin D3 (P105) as GTIOC1A output
  PFS_P105 = 0x00000000;           // Clear
  PFS_P105 = (1 << 6) | (1 << 5) | 0x13;   // PMR=1 (peripheral), PDR=1 (output), PSEL=0x13 (GPT)

  // 4. GPT1 PWM Configuration (Saw-wave PWM)
  GTCR = 0x00000000;
  GTCR |= (1UL << 16);     // PWM mode (saw wave)

  // === Frequency settings (change here for testing) ===
  GTPR   = 479;            // Period → \~100 kHz (good starting point)
  GTCCR0 = 240;            // Duty cycle \~50%

  // Max frequency test (uncomment if you want to try)
  // GTPR   = 1;           // Theoretical \~24 MHz
  // GTCCR0 = 1;

  // 5. Configure GTIOC1A (Channel A) output
  GTIOR = 0x00000000;
  GTIOR |= 0x00000001;     // GTIOCA output enable
  GTIOR |= (0x6UL << 8);   // PWM output mode

  // 6. Start timer
  GTSTR = 0x00000002UL;

  Serial.println("PWM started on D3! Connect oscilloscope now.");
  Serial.println("If still no output, try changing PFS_P105 value to 0x0000000B or 0x00000023");
}

void loop() {
  delay(2000);
  Serial.print("Current Period: ");
  Serial.println(GTPR);
}  R_GPT1_GTIOR |= (0x6UL << 8); // PWM output mode (toggle on compare match for PWM)

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
