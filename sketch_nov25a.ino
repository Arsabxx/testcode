#include <WiFiS3.h>

// WiFi Credentials (from PDF page 11)
const char* ssid     = "TP-Link_4B2C";
const char* password = "43545148";
const char* host     = "192.168.0.181"; // Target Switch IP
const int   port     = 80;              // Standard HTTP port

// Hardware Pin Definitions
const int buttonPin = 8;  // Button connected to Pin 8 (per PDF page 10)
int cnt = 0;              // Counter to track button presses
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open

  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor

  Serial.println("\n--- ELE2003 Week 9: WiFi Switch Challenge ---");
  
  // Start WiFi connection
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Check connection status
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("Arduino IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Control Options:");
  Serial.println(" - Type '1' in Serial Monitor to turn ON");
  Serial.println(" - Type '0' in Serial Monitor to turn OFF");
  Serial.println(" - Press the physical button to toggle");
}

void loop() {
  // --- Method 1: Serial Monitor Control ---
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '1') {
      Serial.println("Serial Input: ON");
      sendHttpRequest(true);
    } else if (input == '0') {
      Serial.println("Serial Input: OFF");
      sendHttpRequest(false);
    }
  }

  // --- Method 2: Physical Button Control (Coding Challenge Logic) ---
  bool currentButtonState = digitalRead(buttonPin);
  
  // Detect falling edge (button press)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(50); // Simple debounce delay
    cnt++;     // Increment counter
    
    Serial.print("Button Pressed! Count: ");
    Serial.println(cnt);
    
    // If count is odd -> ON, if count is even -> OFF
    if (cnt % 2 != 0) {
      sendHttpRequest(true);
    } else {
      sendHttpRequest(false);
    }
  }
  lastButtonState = currentButtonState;
}

/**
 * Sends an HTTP GET request to the smart switch
 * Format: GET /rpc/Switch.set?id=0&on=true HTTP/1.1
 */
void sendHttpRequest(bool turnOn) {
  WiFiClient client;
  
  Serial.print("Connecting to switch at ");
  Serial.print(host);
  
  if (client.connect(host, port)) {
    Serial.println("... Connected!");

    // Construct the URL string based on the state
    String stateStr = (turnOn ? "true" : "false");
    String url = "/rpc/Switch.set?id=0&on=" + stateStr;

    // Send HTTP Request (per PDF page 9-10 instructions)
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.1");
    
    client.print("Host: ");
    client.println(host);
    
    client.println("Connection: close");
    client.println(); // CR LF: Required empty line to end HTTP header

    Serial.print("Request Sent: ");
    Serial.println(url);

    // Read the server response (optional, helpful for debugging)
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        // Serial.println(line); // Uncomment to see full response
      }
    }
    client.stop();
    Serial.println("Request Finished.\n");
  } else {
    Serial.println("... Connection Failed! Check Switch IP.");
  }
}
