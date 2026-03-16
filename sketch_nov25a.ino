#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10    // SDA / SS 腳位（同你原本程式一樣）
#define RST_PIN  4    // Reset 腳位（同你原本一樣）

MFRC522 rfid(SS_PIN, RST_PIN);   // 建立 MFRC522 物件

void setup() {
  Serial.begin(115200);
  while (!Serial);   // 等 Serial 準備好（特別對 Leonardo/Micro 有用）

  SPI.begin();                  // 初始化 SPI
  rfid.PCD_Init();              // 初始化 MFRC522

  Serial.println("RFID UID 測試程式");
  Serial.println("請將卡片或鎖匙靠近讀卡器...");
  Serial.println("----------------------------------------");
  Serial.println("版本檢查: ");
  rfid.PCD_DumpVersionToSerial();  // 顯示 MFRC522 版本，確認模組正常
  Serial.println("----------------------------------------");
}

void loop() {
  // 檢查有無新卡
  if (!rfid.PICC_IsNewCardPresent()) {
    return;   // 無卡就直接跳過
  }

  // 嘗試讀取卡片
  if (!rfid.PICC_ReadCardSerial()) {
    return;   // 讀取失敗就跳過
  }

  // 成功讀到卡！印出詳細資訊
  Serial.println("=== 偵測到卡片 ===");
  
  // 顯示卡片類型
  Serial.print("卡片類型: ");
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // 顯示 UID（最重要部分）
  Serial.print("UID (HEX): ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      Serial.print(" 0");           // 補零讓格式好看
    } else {
      Serial.print(" ");
    }
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // 也印出常用於程式碼的格式（直接 copy paste 用）
  Serial.print("程式碼格式: {");
  for (byte i = 0; i < rfid