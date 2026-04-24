#include <ArduinoBLE.h>

// BLE Service and Characteristic UUIDs
// These are standard unique identifiers for the BLE service
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 

// BLE Characteristic: "19B10001" allows remote device to Write (1 or 0)
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // Use the onboard LED (Pin 13)

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor

  pinMode(ledPin, OUTPUT); // Initialize LED pin as output

  // Initialize BLE hardware
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set local name that will appear on your phone
  BLE.setLocalName("UnoR4_BLE_Control");
  BLE.setAdvertisedService(ledService);

  // Add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);

  // Set initial value for the characteristic
  switchCharacteristic.writeValue(0);

  // Start advertising so phones can find it
  BLE.advertise();

  Serial.println("BLE Device Active, waiting for connections...");
  Serial.println("Use nRF Connect app to connect and write '01' to turn on LED.");
}

void loop() {
  // Listen for BLE centrals (e.g., your smartphone)
  BLEDevice central = BLE.central();

  // If a phone is connected to the Arduino
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // If the phone has written a new value to the characteristic
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   
          Serial.println("BLE Command: LED ON");
          digitalWrite(ledPin, HIGH);         // Turn on LED
        } else {                              
          Serial.println("BLE Command: LED OFF");
          digitalWrite(ledPin, LOW);          // Turn off LED
        }
      }
    }

    // When the phone disconnects
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
