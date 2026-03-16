C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:26:1: error: 'Servo' does not name a type
 Servo barrier;
 ^~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:27:1: error: 'MFRC522' does not name a type
 MFRC522 rfid(SS_PIN, RST_PIN);
 ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:28:1: error: 'U8G2_SSD1306_128X64_NONAME_F_HW_I2C' does not name a type
 U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);  // OLED I2C (SSD1306 128x64)
 ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'void setup()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:39:11: error: 'LED_PIN' was not declared in this scope
   pinMode(LED_PIN, OUTPUT);
           ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:39:11: note: suggested alternative: 'RST_PIN'
   pinMode(LED_PIN, OUTPUT);
           ^~~~~~~
           RST_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:42:3: error: 'barrier' was not declared in this scope
   barrier.attach(SERVO_PIN);
   ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:45:3: error: 'SPI' was not declared in this scope
   SPI.begin();
   ^~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:45:3: note: suggested alternative: 'PI'
   SPI.begin();
   ^~~
   PI
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:46:3: error: 'rfid' was not declared in this scope
   rfid.PCD_Init();             // Initialize MFRC522
   ^~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:46:3: note: suggested alternative: 'rand'
   rfid.PCD_Init();             // Initialize MFRC522
   ^~~~
   rand
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:48:3: error: 'u8g2' was not declared in this scope
   u8g2.begin();
   ^~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:48:3: note: suggested alternative: 'log2'
   u8g2.begin();
   ^~~~
   log2
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'void loop()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:59:18: error: 'LED_PIN' was not declared in this scope
     digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate "scan card"
                  ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:59:18: note: suggested alternative: 'RST_PIN'
     digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate "scan card"
                  ^~~~~~~
                  RST_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:66:9: error: 'rfid' was not declared in this scope
     if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
         ^~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:66:9: note: suggested alternative: 'rand'
     if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
         ^~~~
         rand
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:87:9: error: 'barrier' was not declared in this scope
         barrier.write(openAngle);
         ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:90:22: error: 'LED_PIN' was not declared in this scope
         digitalWrite(LED_PIN, LOW);
                      ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:90:22: note: suggested alternative: 'RST_PIN'
         digitalWrite(LED_PIN, LOW);
                      ^~~~~~~
                      RST_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:105:5: error: 'barrier' was not declared in this scope
     barrier.write(closedAngle);
     ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'int getDistance()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:118:29: error: 'THERMISTOR_PIN' was not declared in this scope
   int rawValue = analogRead(THERMISTOR_PIN);
                             ^~~~~~~~~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:118:29: note: suggested alternative: 'RST_PIN'
   int rawValue = analogRead(THERMISTOR_PIN);
                             ^~~~~~~~~~~~~~
                             RST_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'void displayMessage(arduino::String, arduino::String)':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:152:3: error: 'u8g2' was not declared in this scope
   u8g2.clearBuffer();
   ^~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:152:3: note: suggested alternative: 'log2'
   u8g2.clearBuffer();
   ^~~~
   log2
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:153:16: error: 'u8g2_font_ncenB10_tr' was not declared in this scope
   u8g2.setFont(u8g2_font_ncenB10_tr);
                ^~~~~~~~~~~~~~~~~~~~
exit status 1

Compilation error: 'Servo' does not name a type
