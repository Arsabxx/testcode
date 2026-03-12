#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define DHT_PIN     2       // KY-015 DHT11 Signal
#define DHT_TYPE    DHT11
#define DS18B20_PIN 4       // KY-001 DS18B20 DQ
#define HEART_PIN   A0      // KY-039 Heartbeat analog

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;

// Heartbeat variables (更寬鬆版本)
static int previousRaw = 0;
static unsigned long lastBeat = 0;
static int bpm = 0;
static int baseline = 0;              // 自動跟蹤當前水平
static int deltaThreshold = 20;       // 降低門檻，更容易偵測（可調 15~30）
static int minChange = 15;            // 最小變化率輔助（更寬鬆）
static int confirmCount = 0;          // 連續確認計數器
static const int confirmNeeded = 2;   // 需要連續 2 次偵測才計 beat

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("=== All Sensors Test - 更寬鬆 Heartbeat 偵測版 ===");
  Serial.println("KY-001 Temp | KY-015 Temp+Hum | KY-039 Raw/BPM | MPU-6500 Accel(g) X Y Z");

  dht.begin();
  ds18b20.begin();
  if (ds18b20.getDeviceCount() == 0) {
    Serial.println("KY-001: No sensor found!");
  }

  if (!mpu.begin()) {
    Serial.println("MPU-6500: Failed! Check wiring.");
  } else {
    Serial.println("MPU-6500: Connected!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }

  Serial.println("Start. Clip finger on KY-039 + cover light well.");
  Serial.println("Format: TempDS C | TempDHT C Hum% | Raw BPM | Accel X Y Z g");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // 1. KY-001 DS18B20
  ds18b20.requestTemperatures();
  float tempDS = ds18b20.getTempCByIndex(0);
  if (tempDS == DEVICE_DISCONNECTED_C) tempDS = -99.9;

  // 2. KY-015 DHT11
  float hum = dht.readHumidity();
  float tempDHT = dht.readTemperature();
  if (isnan(hum) || isnan(tempDHT)) {
    hum = tempDHT = -99.9;
  }

  // 3. KY-039 Heartbeat - 更寬鬆偵測
  int heartRaw = analogRead(HEART_PIN);

  // 簡單濾波（5 次平均）
  static long sumRaw = 0; static int cnt = 0;
  sumRaw += heartRaw; cnt++;
  if (cnt >= 5) {
    heartRaw = sumRaw / 5;
    sumRaw = 0; cnt = 0;
  }

  // 更新 baseline（平滑跟蹤）
  baseline = (baseline * 9 + heartRaw) / 10;

  // 計算差值
  int diff = heartRaw - baseline;

  // 更寬鬆偵測條件
  bool potentialPeak = (diff > deltaThreshold) || (diff > minChange && heartRaw > previousRaw);

  if (potentialPeak) {
    confirmCount++;
    if (confirmCount >= confirmNeeded) {
      unsigned long interval = millis() - lastBeat;
      if (interval > 300 && interval < 2500) {  // 更寬鬆：24~200 BPM
        bpm = 60000UL / interval;
        lastBeat = millis();
        Serial.print(" [Beat!] ");
        confirmCount = 0;  // 重置計數
      }
    }
  } else {
    confirmCount = 0;  // 連續性中斷，重置
  }

  previousRaw = heartRaw;

  // 4. MPU-6500 Accelerometer
  sensors_event_t a, g, temp;
  float accX = 0, accY = 0, accZ = 0;
  if (mpu.getEvent(&a, &g, &temp)) {
    accX = a.acceleration.x / 9.81f;
    accY = a.acceleration.y / 9.81f;
    accZ = a.acceleration.z / 9.81f;
  }

  // 輸出全部數據
  Serial.print(tempDS, 1); Serial.print(" C | ");
  Serial.print(tempDHT, 1); Serial.print(" C "); Serial.print(hum, 0); Serial.print("% | ");
  Serial.print(heartRaw); Serial.print(" raw "); Serial.print(bpm); Serial.print(" BPM | ");
  Serial.print("Accel: "); Serial.print(accX, 2); Serial.print(" ");
  Serial.print(accY, 2); Serial.print(" "); Serial.print(accZ, 2); Serial.println(" g");

  delay(2000);  // 每 2 秒一輪
}