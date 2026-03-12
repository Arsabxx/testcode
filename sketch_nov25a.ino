#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define DHT_PIN     2
#define DHT_TYPE    DHT11
#define DS18B20_PIN 4
#define HEART_PIN   A0

DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);  // 較高 baud rate 方便看多數據
  while (!Serial) delay(10);

  Serial.println("=== All Sensors Test Start ===");
  Serial.println("KY-001 (DS18B20) + KY-015 (DHT11) + KY-039 (Heartbeat) + MPU-6500");

  // 初始化 DHT11
  dht.begin();

  // 初始化 DS18B20
  ds18b20.begin();
  if (ds18b20.getDeviceCount() == 0) {
    Serial.println("DS18B20: No sensor found!");
  }

  // 初始化 MPU-6500
  if (!mpu.begin()) {
    Serial.println("MPU-6500: Failed to find chip! Check wiring / address.");
    // 如果失敗，試 mpu.begin(0x69) 如果 AD0 高
  } else {
    Serial.println("MPU-6500: Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }

  Serial.println("Setup complete. Monitoring every 2 seconds...");
  Serial.println("Format: TempDS18(°C) | TempDHT(°C) Hum(%) | HeartRaw | BPM | Accel(g) X Y Z | Gyro(°/s) X Y Z");
  Serial.println("------------------------------------------------------------");
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

  // 3. KY-039 Heartbeat (簡化 raw + BPM)
  int heartRaw = analogRead(HEART_PIN);
  static unsigned long lastBeat = 0;
  static int bpm = 0;
  static int threshold = 600;  // 調節：靜止 raw ≈100-300，手指夾 ≈500-900
  if (heartRaw > threshold && millis() - lastBeat > 300) {
    unsigned long interval = millis() - lastBeat;
    if (interval > 300 && interval < 2000) {
      bpm = 60000 / interval;
      lastBeat = millis();
    }
  }

  // 4. MPU-6500 Accelerometer & Gyro
  sensors_event_t a, g, temp;
  float accX = 0, accY = 0, accZ = 0;
  float gyroX = 0, gyroY = 0, gyroZ = 0;
  if (mpu.getEvent(&a, &g, &temp)) {
    accX = a.acceleration.x / 9.81;  // 轉 g
    accY = a.acceleration.y / 9.81;
    accZ = a.acceleration.z / 9.81;
    gyroX = g.gyro.x;  // °/s
    gyroY = g.gyro.y;
    gyroZ = g.gyro.z;
  }

  // Serial Monitor 輸出全部
  Serial.print(tempDS, 1); Serial.print(" °C | ");
  Serial.print(tempDHT, 1); Serial.print(" °C "); Serial.print(hum, 0); Serial.print("% | ");
  Serial.print(heartRaw); Serial.print(" raw | ");
  Serial.print(bpm); Serial.print(" BPM | ");
  Serial.print("Accel: "); Serial.print(accX, 2); Serial.print(" "); Serial.print(accY, 2); Serial.print(" "); Serial.print(accZ, 2); Serial.print(" g | ");
  Serial.print("Gyro: "); Serial.print(gyroX, 2); Serial.print(" "); Serial.print(gyroY, 2); Serial.print(" "); Serial.print(gyroZ, 2); Serial.println(" °/s");

  delay(2000);  // 每 2 秒一輪
}