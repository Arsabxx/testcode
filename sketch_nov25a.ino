/*
 * ELE2003-2526 Mechatronics Systems - Week 9 Coding Challenge
 * WiFi Control: Button → Arduino Uno R4 WiFi → TP-Link Smart Switch
 * 
 * Board: Arduino Uno R4 WiFi
 * Function: Press button once → toggle the smart switch ON / OFF
 * Switch IP: 192.168.0.181
 */

#include <WiFiS3.h>          // Important: Use WiFiS3 for Uno R4 WiFi

// ================== WiFi Settings ==================
const char* ssid = "YOUR_WIFI_SSID";           // ← Change to your WiFi name
const char* password = "YOUR_WIFI_PASSWORD";   // ← Change to your WiFi password

// ================== Switch Settings ==================
const char* switchIP = "192.168.0.181";        // From your photo
const int   switchPort = 9999;                 // Kasa local control port

// ================== Button Settings ==================
const int buttonPin = 2;                       // Recommended pins: 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0-A5
                                               // Connect button between this pin and GND

// Variables for button (debounce)
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;        // 50ms debounce

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);            // Use internal pull-up

  Serial.println("\n=== Uno R4 WiFi - TP-Link Kasa Switch Controller ===");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Uno R4 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Target Switch IP: ");
  Serial.println(switchIP);
  Serial.println("Ready! Press the button to toggle the switch.");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Detect button press (falling edge + debounce)
  if (buttonState == LOW && lastButtonState == HIGH && 
      (millis() - lastDebounceTime) > debounceDelay) {
    
    lastDebounceTime = millis();
    Serial.println("\n[EVENT] Button Pressed! Toggling Smart Switch...");

    toggleKasaSwitch();
  }

  lastButtonState = buttonState;
  delay(10);
}

// ====================== Toggle Function ======================
void toggleKasaSwitch() {
  WiFiClient client;

  if (client.connect(switchIP, switchPort)) {
    Serial.println("Connected to switch... sending toggle command");

    // Simple toggle command (JSON + XOR encryption)
    const char* cmd = "{\"system\":{\"set_relay_state\":{\"state\":-1}}}";  // -1 = toggle

    // Encrypt the command (TP-Link Kasa uses simple XOR 0xAB)
    String encrypted = encryptKasa(cmd);

    client.print(encrypted);
    client.stop();

    Serial.println("Toggle command sent successfully!");
  } 
  else {
    Serial.println("Connection failed! Check switch IP and WiFi.");
  }
}

// ====================== Kasa Encryption Function ======================
String encryptKasa(const char* str) {
  String result = "";
  int key = 0xAB;                     // Fixed key for Kasa local protocol
  int len = strlen(str);

  // First byte is the length (big endian, but we send as string)
  result += (char)((len >> 8) & 0xFF);
  result += (char)(len & 0xFF);

  for (int i = 0; i < len; i++) {
    result += (char)(str[i] ^ key);
    key = result[result.length() - 1];   // Update key for next byte
  }
  return result;
}