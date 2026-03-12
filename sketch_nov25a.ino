#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define DHT_PIN     2       // KY-015 DHT11 Signal pin
#define DHT_TYPE    DHT11
#define DS18B20_PIN 4       // KY-001 DS18B20 DQ pin
#define HEART_PIN   A0      // KY-039 Heartbeat analog pin

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;

// Heartbeat variables (寬鬆變化率偵測，適合你嘅反射模式)
static float oldValue     = 0.0;
static float alpha        = 0.70;          // 平滑係數，0.65~0.8 之間調（低 = 捕捉細變化）
static float maxChange    = 0.0;           // 追蹤最大變化，慢慢衰減
static unsigned long lastBeat = 0;
static int bpm            = 0;
static int minBeatInterval = 250;          // 最短間隔 ms（避免噪音誤觸）
static int maxBeatInterval = 4000;         // 最長間隔 ms（容許慢心率）

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\n=== 所有 Sensor 完整測試 (2026-03-12) ===");
  Serial.println("KY-001 Temp | KY-015 Temp+Hum | KY-039 Heartbeat (Raw/BPM) | MPU-6500 Accel (g)");
  Serial.println("Debug: Raw | Smoothed | Change | BPM");

  // 初始化 DHT11
  dht.begin();

  // 初始化 DS18B20
  ds18b20.begin();
  if (ds18b20.getDeviceCount() == 0) {
    Serial.println("KY-001: 未找到 DS18B20！檢查接線");
  }

  // 初始化 MPU-6500 (I2C)
  if (!mpu.begin()) {
    Serial.println("MPU-6500: 初始化失敗！檢查 SDA(A4)/SCL(A5)/VCC");
  } else {
    Serial.println("MPU-6500: 已連接！");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }

  Serial.println("開始監測... 請夾住 KY-039 手指 + 用手/黑布完全遮光！");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // 1. KY-001 DS18B20 溫度
  ds18b20.requestTemperatures();
  float tempDS = ds18b20.getTempCByIndex(0);
  if (tempDS == DEVICE_DISCONNECTED_C) tempDS = -99.9;

  // 2. KY-015 DHT11 溫濕度
  float hum = dht.readHumidity();
  float tempDHT = dht.readTemperature();
  if (isnan(hum) || isnan(tempDHT)) {
    hum = tempDHT = -99.9;
  }

  // 3. KY-039 Heartbeat - 變化率偵測（超寬鬆版）
  int raw = analogRead(HEART_PIN);

  // 平滑濾波
  float smoothed = alpha * oldValue + (1 - alpha) * raw;
  float change   = smoothed - oldValue;
  oldValue = smoothed;

  // 偵測正變化（上升峰，適合你夾手指 raw 上升情況）
  if (change > maxChange && millis() - lastBeat > minBeatInterval) {
    maxChange = change;
    unsigned long interval = millis() - lastBeat;
    if (interval < maxBeatInterval) {
      bpm = 60000UL / interval;
      lastBeat = millis();
      Serial.print(" [偵測到心跳!] Change: "); Serial.print(change, 2);
      Serial.print(" 新 BPM: "); Serial.println(bpm);
    }
  }

  // 衰減 maxChange（避免永遠鎖死高值）
  maxChange *= 0.92;   // 衰減率，可調 0.90~0.98

  // 4. MPU-6500 加速度
  sensors_event_t a, g, temp;
  float accX = 0, accY = 0, accZ = 0;
  if (mpu.getEvent(&a, &g, &temp)) {
    accX = a.acceleration.x / 9.81f;
    accY = a.acceleration.y / 9.81f;
    accZ = a.acceleration.z / 9.81f;
  }

  // 輸出主要數據
  Serial.print(tempDS, 1); Serial.print(" C | ");
  Serial.print(tempDHT, 1); Serial.print(" C "); Serial.print(hum, 0); Serial.print("% | ");
  Serial.print(raw); Serial.print(" raw "); Serial.print(bpm); Serial.print(" BPM | ");
  Serial.print("Accel X/Y/Z: "); 
  Serial.print(accX, 2); Serial.print(" "); 
  Serial.print(accY, 2); Serial.print(" "); 
  Serial.print(accZ, 2); Serial.println(" g");

  // 心跳 debug 資訊
  Serial.print("Debug: Raw "); Serial.print(raw);
  Serial.print(" | Smoothed "); Serial.print(smoothed, 1);
  Serial.print(" | Change "); Serial.print(change, 2);
  Serial.print(" | BPM "); Serial.println(bpm);

  Serial.println("---");
  delay(2000);  // 每 2 秒更新一次
}