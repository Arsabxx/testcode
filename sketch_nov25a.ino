#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10     // SDA / SS pin (你原本設定)
#define RST_PIN  4     // Reset pin (你原本設定)

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);   // 等 Serial 穩定

  SPI.begin();                  // 初始化 SPI 總線
  rfid.PCD_Init();              // 初始化 MFRC522

  Serial.println(F("=== RFID UID Test Program ==="));
  Serial.println(F("Please place your card or key fob near the reader..."));
  Serial.println(F("----------------------------------------"));

  // 顯示 MFRC522 版本，確認模組是否正常
  rfid.PCD_DumpVersionToSerial();
  if (rfid.PCD_PerformSelfTest()) {
    Serial.println(F("Self-test: PASSED"));
  } else {
    Serial.println(F("Self-test: FAILED - Check wiring / module"));
  }
  Serial.println(F("----------------------------------------"));
}

void loop() {
  // 檢查是否有新卡
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // 嘗試讀取卡片
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // 讀到卡了！輸出詳細資訊
  Serial.println(F("=== Card Detected ==="));

  // 卡片類型
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print(F("Card Type: "));
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // UID (HEX 格式)
  Serial.print(F("UID (HEX): "));
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print(F("0"));
    }
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  // 直接輸出可 copy 去陣列的格式
  Serial.print(F("Copy-paste format: {"));
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(F("0x"));
    if (rfid.uid.uidByte[i] < 0x10) Serial.print(F("0"));
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) Serial.print(F(", "));
  }
  Serial.println(F("}"));

  // UID 長度（通常 4 或 7 bytes）
  Serial.print(F("UID size: "));
  Serial.print(rfid.uid.size);
  Serial.println(F(" bytes"));

  Serial.println(F("----------------------------------------"));

  // 停止讀這張卡
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();  // 部分卡需要

  delay(1500);  // 避免太快重複讀
}