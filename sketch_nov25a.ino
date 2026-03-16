#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN  9   // 建議用 9 做測試，如果你想用 4 就改返 4

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("RFID UID 測試開始...");
  Serial.println("請放卡/鎖匙靠近讀卡器");
  rfid.PCD_DumpVersionToSerial();  // 顯示模組版本，確認硬件 OK
  Serial.println("----------------------------------------");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println("=== 讀到卡！ ===");
  Serial.print("UID (HEX): ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("直接 copy 格式: {");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print("0x");
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) Serial.print(", ");
  }
  Serial.println("}");

  rfid.PICC_HaltA();
  delay(1500);  // 避免太快重複
}