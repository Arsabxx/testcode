C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'void setup()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:45:11: error: 'LED_PIN' was not declared in this scope
   pinMode(LED_PIN, OUTPUT);
           ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:45:11: note: suggested alternative: 'SS_PIN'
   pinMode(LED_PIN, OUTPUT);
           ^~~~~~~
           SS_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'void loop()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:65:18: error: 'LED_PIN' was not declared in this scope
     digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate "scan card"
                  ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:65:18: note: suggested alternative: 'SS_PIN'
     digitalWrite(LED_PIN, HIGH);                // Turn on LED to indicate "scan card"
                  ^~~~~~~
                  SS_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:96:22: error: 'LED_PIN' was not declared in this scope
         digitalWrite(LED_PIN, LOW);
                      ^~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:96:22: note: suggested alternative: 'SS_PIN'
         digitalWrite(LED_PIN, LOW);
                      ^~~~~~~
                      SS_PIN
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino: In function 'int getDistance()':
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:124:29: error: 'THERMISTOR_PIN' was not declared in this scope
   int rawValue = analogRead(THERMISTOR_PIN);
                             ^~~~~~~~~~~~~~
C:\Users\29238929\AppData\Local\Temp\.arduinoIDE-unsaved2026214-12916-15qv9o0.ub4z\sketch_mar14a\sketch_mar14a.ino:124:29: note: suggested alternative: 'TRIG_PIN'
   int rawValue = analogRead(THERMISTOR_PIN);
                             ^~~~~~~~~~~~~~
                             TRIG_PIN
exit status 1

Compilation error: 'LED_PIN' was not declared in this scope
