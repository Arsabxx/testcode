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

// Heartbeat variables (adaptive for your case: no finger ~600, finger >900)
static int previousRaw = 0;
static unsigned long lastBeat = 0;
static int bpm = 0;
static int baseline = 0;          // 自動跟蹤夾手指高值
static int deltaThreshold = 40;   // 調整呢個！波幅小就 20-30，大就 50-80（用 Plotter 睇）

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("=== All Sensors Full Test (Serial Monitor) ===");
  Serial.println("KY-001 Temp | KY-015 Temp+Hum | KY-039 Heart (Raw/BPM) | MPU-6500 Accel(g) X Y Z");

  // Init DHT11
  dht.begin();

  // Init DS18B20
  ds18b20.begin();
  if (ds18b20.getDeviceCount() == 0) {
    Serial.println("KY-001: No sensor found! Check wiring.");
  }

  // Init MPU-6500 (I2C)
  if (!mpu.begin()) {
    Serial.println("MPU-6500: Failed! Check SDA(A4)/SCL(A5)/VCC.");
    // 如果地址錯，試 mpu.begin(0x69);
  } else {
    Serial.println("MPU-6500: Connected!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }

  Serial.println("Start monitoring... Clip finger on KY-039 + cover light!");
  Serial.println("Format: TempDS | TempDHT Hum% | Raw BPM | Accel X Y Z (g)");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // 1. KY-001 DS18B20 Temperature
  ds18b20.requestTemperatures();
  float tempDS = ds18b20.getTempCByIndex(0);
  if (tempDS == DEVICE_DISCONNECTED_C) tempDS = -99.9;

  // 2. KY-015 DHT11
  float hum = dht.readHumidity();
  float tempDHT = dht.readTemperature();
  if (isnan(hum) || isnan(tempDHT)) {
    hum = tempDHT = -99.9;
  }

  // 3. KY-039 Heartbeat (adaptive peak detection for your inverted/high signal)
  int heartRaw = analogRead(HEART_PIN);

  // Simple filter (average 5 reads)
  static long sumRaw = 0; static int cnt = 0;
  sumRaw += heartRaw; cnt++;
  if (cnt >= 5) {
    heartRaw = sumRaw / 5;
    sumRaw = 0; cnt = 0;
  }

  // Update baseline (smooth tracking of current level)
  baseline = (baseline * 9 + heartRaw) / 10;

  // Detect small rise above baseline (reflection peak when finger on)
  if (heartRaw > baseline + deltaThreshold && previousRaw <= baseline + deltaThreshold) {
    unsigned long interval = millis() - lastBeat;
    if (interval > 400 && interval < 2000) {  // 30-150 BPM range
      bpm = 60000UL / interval;
      lastBeat = millis();
      Serial.print(" [Beat!] ");
    }
  }
  previousRaw = heartRaw;

  // 4. MPU-6500 Accelerometer
  sensors_event_t a, g, temp;
  float accX = 0, accY = 0, accZ = 0;
  if (mpu.getEvent(&a, &g, &temp)) {
    accX = a.acceleration.x / 9.81f;  // to g
    accY = a.acceleration.y / 9.81f;
    accZ = a.acceleration.z / 9.81f;
  }

  // Print all to Serial Monitor
  Serial.print(tempDS, 1); Serial.print(" C | ");
  Serial.print(tempDHT, 1); Serial.print(" C "); Serial.print(hum, 0); Serial.print("% | ");
  Serial.print(heartRaw); Serial.print(" raw "); Serial.print(bpm); Serial.print(" BPM | ");
  Serial.print("Accel: "); Serial.print(accX, 2); Serial.print(" "); 
  Serial.print(accY, 2); Serial.print(" "); Serial.print(accZ, 2); Serial.println(" g");

  delay(2000);  // 每 2 秒更新一次
}