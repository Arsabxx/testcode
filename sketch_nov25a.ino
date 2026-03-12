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

// Heartbeat variables (更寬鬆 + 變化率偵測)
static float oldValue = 0.0;
static float alpha = 0.75;              // 平滑係數（0.7~0.85，愈低愈平滑）
static float maxChange = 0.0;           // 峰值變化衰減
static unsigned long lastBeat = 0;
static int bpm = 0;
static int minDelayBetweenBeats = 300;  // 最小間隔 ms（避免噪音）
static int maxDelayBetweenBeats = 3000; // 最大間隔 ms（避免太慢誤判）

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("=== All Sensors Test - 更寬鬆變化率 Heartbeat 偵測版 ===");
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
  Serial.println("Debug: Raw | Smoothed | Change | BPM");
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

  // 3. KY-039 Heartbeat - 變化率偵測（更寬鬆）
  int raw = analogRead(HEART_PIN);

  // 平滑濾波
  float smoothed = alpha * oldValue + (1 - alpha) * raw;
  float change = smoothed - oldValue;
  oldValue = smoothed;

  // 偵測正變化（上升峰，適合你反射模式）
  if (change > maxChange && millis() - lastBeat > minDelayBetweenBeats) {
    maxChange = change;
    unsigned long interval = millis() - lastBeat;
    if (interval < maxDelayBetweenBeats) {  // 避免無限大
      bpm = 60000UL / interval;
      lastBeat = millis();
      Serial.print(" [Beat!] Change: "); Serial.print(change, 2);
      Serial.print(" BPM updated to: "); Serial.println(bpm);
    }
  }

  // 衰減 maxChange（慢慢降低，避免永遠高）
  maxChange *= 0.95;  // 衰減率，可調 0.9~0.98

  // 4. MPU-6500 Accelerometer
  sensors_event_t a, g, temp;
  float accX = 0, accY = 0, accZ = 0;
  if (mpu.getEvent(&a, &g, &temp)) {
    accX = a.acceleration.x / 9.81f;
    accY = a.acceleration.y / 9.81f;
    accZ = a.acceleration.z / 9.81f;
  }

  // 輸出全部
  Serial.print(tempDS, 1); Serial.print(" C | ");
  Serial.print(tempDHT, 1); Serial.print(" C "); Serial.print(hum, 0); Serial.print("% | ");
  Serial.print(raw); Serial.print(" raw "); Serial.print(bpm); Serial.print(" BPM | ");
  Serial.print("Accel: "); Serial.print(accX, 2); Serial.print(" ");
  Serial.print(accY, 2); Serial.print(" "); Serial.print(accZ, 2); Serial.println(" g");

  // Debug 心跳
  Serial.print("Debug: Raw "); Serial.print(raw);
  Serial.print(" | Smoothed "); Serial.print(smoothed, 1);
  Serial.print(" | Change "); Serial.print(change, 2);
  Serial.print(" | BPM "); Serial.println(bpm);

  delay(2000);
}