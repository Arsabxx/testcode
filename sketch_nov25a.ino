#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>  // for log()

// Pin definitions
#define TRIG_PIN     5
#define ECHO_PIN     6
#define SS_PIN      10
#define RST_PIN      4
#define SERVO_PIN    9
#define LED_PIN      7
#define THERMISTOR_PIN A0

// OLED 配置（128x64 I2C）
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1   // 無 reset pin，用 -1
#define SCREEN_ADDRESS 0x3C  // 預設 0x3C，如果 scanner 顯示 0x3D 就改成 0x3D

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Thermistor constants
const float nominal_resistance = 10000;
const float nominal_temperature = 25;
const float beta_coefficient = 3950;
const float series_resistor = 10000;

// Adjustable parameters
const int triggerDist   = 120;
const int openAngle     = 0;
const int closedAngle   = 90;
const int gateOpenTime  = 8000;

// Authorized UIDs - 改成你真實 UID
byte authorizedUIDs[][4] = {
  {0xA1, 0xB2, 0xC3, 0xD4},
  {0xE5, 0xF6, 0xA7, 0xB8}
};
const int numAuthorized = 2;

Servo barrier;
MFRC522 rfid(SS_PIN, RST_PIN);

bool carDetected = false;
bool gateOpen = false;
unsigned long openStartTime = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  barrier.attach(SERVO_PIN);
  barrier.write(closedAngle);
  
  SPI.begin();
  rfid.PCD_Init();
  
  // 初始化 OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // 如果失敗，死循環（可移除）
  }
  
  display.clearDisplay();
  display.setTextSize(1);               // 文字大小 1
  display.setTextColor(SSD1306_WHITE);  // 白字（OLED 通常白光）
  display.setCursor(0, 10);
  display.println(F("Parking Entrance"));
  display.setCursor(0, 30);
  display.println(F("Approach Vehicle"));
  display.display();                    // 送出畫面
  
  Serial.println("System Ready - Waiting for vehicle");
}

void loop() {
  int distance = getDistance();
  
  if (distance > 0 && distance < triggerDist && !carDetected && !gateOpen) {
    carDetected = true;
    digitalWrite(LED_PIN, HIGH);
    displayMessage("Vehicle Detected", "Please Scan Card");
    Serial.println("Car detected! Please scan RFID card");
  }
  
  if (carDetected && !gateOpen) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      Serial.print("Card UID: ");
      for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();
      
      bool valid = false;
      for (int i = 0; i < numAuthorized; i++) {
        if (memcmp(rfid.uid.uidByte, authorizedUIDs[i], 4) == 0) {
          valid = true;
          break;
        }
      }
      
      if (valid) {
        displayMessage("Access Granted", "Gate Opening");
        barrier.write(openAngle);
        gateOpen = true;
        openStartTime = millis();
        digitalWrite(LED_PIN, LOW);
        Serial.println("Valid card! Gate OPEN");
      } else {
        displayMessage("Invalid Card", "Try Again");
        Serial.println("Invalid card!");
        delay(2000);
        displayMessage("Vehicle Detected", "Please Scan Card");
      }
      
      rfid.PICC_HaltA();
    }
  }
  
  if (gateOpen && (millis() - openStartTime > gateOpenTime)) {
    barrier.write(closedAngle);
    gateOpen = false;
    carDetected = false;
    displayMessage("Parking Entrance", "Approach Vehicle");
    Serial.println("Gate CLOSED");
  }
  
  delay(100);
}

// 溫度補償距離
int getDistance() {
  int rawValue = analogRead(THERMISTOR_PIN);
  if (rawValue <= 0 || rawValue >= 1023) return -1;

  float resistance = series_resistor * (1023.0 / rawValue - 1.0);
  float steinhart = resistance / nominal_resistance;
  steinhart = log(steinhart);
  steinhart /= beta_coefficient;
  steinhart += 1.0 / (nominal_temperature + 273.15);
  steinhart = 1.0 / steinhart;
  float tempC = steinhart - 273.15;

  float speed_of_sound = 331.3 + (0.606 * tempC);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  float distance = (duration * speed_of_sound) / 20000.0;
  if (distance < 2 || distance > 400) return -1;

  return (int)round(distance);
}

// OLED 顯示兩行文字（Adafruit 版）
void displayMessage(String line1, String line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);   // 第一行位置
  display.println(line1);
  display.setCursor(0, 30);   // 第二行位置（可調高低）
  display.println(line2);
  display.display();          // 必須呼叫才更新畫面
}