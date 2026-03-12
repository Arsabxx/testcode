// Breathing Wings Control Code
// Using light sensor to control two motors for wing movement

const int lightSensorPin = A0;    // Light sensor connected to analog pin A0
const int motor1Pin = 9;          // Motor 1 connected to digital pin 9 (PWM)
const int motor2Pin = 10;         // Motor 2 connected to digital pin 10 (PWM)

int lightValue = 0;               // Store light sensor reading
int lightThreshold = 500;         // Light threshold, adjust based on actual conditions
bool isNear = false;              // Flag to indicate if object is near sensor
unsigned long previousMillis = 0; // Record last action time
const long interval = 50;         // Breathing action interval (milliseconds)
int breathDirection = 1;          // Breathing direction: 1=expand, -1=retract
int breathLevel = 0;              // Current breathing level

void setup() {
  Serial.begin(9600);
  
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  analogWrite(motor1Pin, 0);
  analogWrite(motor2Pin, 0);
}

void loop() {
  lightValue = analogRead(lightSensorPin);   // 1. Read light sensor value
  
  Serial.print("Light value: ");
  Serial.println(lightValue);
  // 2. Check if object is near the sensor
  // Lower light value means darker (object is near blocking light)
  // Higher light value means brighter (object is far away)
  if (lightValue < lightThreshold) {
    if (!isNear) {      // Object is near - expand wings and breathe
      Serial.println("Object detected - Expanding wings");
      isNear = true;
      breathLevel = 0;  // Reset breathing level
      breathDirection = 1; // Start expanding
    }

    breatheWings(); 
    
  } else {
    if (isNear) {      // Object is far, retract wings and stop
      Serial.println("Object removed - Retracting wings");
      isNear = false;
    }
    analogWrite(motor1Pin, 0); // stop motor
    analogWrite(motor2Pin, 0);
  }
  delay(100);
}

// simulates wing expansion and retraction
void breatheWings() {
  unsigned long currentMillis = millis();
  
  // Check if it's time to perform the action
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    breathLevel += breathDirection;  // Update breathing level
    
    // Check if direction needs to change
    if (breathLevel >= 255) {
      breathLevel = 255;
      breathDirection = -1; // Start retracting
    } else if (breathLevel <= 0) {
      breathLevel = 0;
      breathDirection = 1;  // Start expanding
    }
    
    // Apply breathing level to both motors
    // Use PWM to control motor speed, creating breathing effect
    analogWrite(motor1Pin, breathLevel);
    analogWrite(motor2Pin, breathLevel);
    
    // Debug information
    Serial.print("Breath level: ");
    Serial.println(breathLevel);
  }
}