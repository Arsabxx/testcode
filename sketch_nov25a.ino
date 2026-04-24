#include <WiFiS3.h>          

const char* ssid = "TP-Link_4B2C";           
const char* password = "43545148";   
const char* switchIP = "192.168.0.181";        
const int   switchPort = 9999;          
const int buttonPin = 2;                       

// Variables for button (debounce)
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;        // 50ms debounce

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);            // Use internal pull-up

  Serial.println("\nSwitch Controller");

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
  Serial.println("Press the button to toggle the switch.");
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Detect button press (falling edge + debounce)
  if (buttonState == LOW && lastButtonState == HIGH && 
      (millis() - lastDebounceTime) > debounceDelay) {
    
    lastDebounceTime = millis();
    Serial.println("\nToggling Switch...");

    toggleKasaSwitch();
  }

  lastButtonState = buttonState;
  delay(10);
}

void toggleKasaSwitch() {
  WiFiClient client;

  if (client.connect(switchIP, switchPort)) {
    Serial.println("Connected to switch... sending toggle command");

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

String encryptKasa(const char* str) {
  String result = "";
  int key = 0xAB;                     // Fixed key for Kasa local protocol
  int len = strlen(str);

  result += (char)((len >> 8) & 0xFF);
  result += (char)(len & 0xFF);

  for (int i = 0; i < len; i++) {
    result += (char)(str[i] ^ key);
    key = result[result.length() - 1];   // Update key for next byte
  }
  return result;
}
}
