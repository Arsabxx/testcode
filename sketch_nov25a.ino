const int POT    = A2;
const int LED1   = 13;
const int LED2   = 3;
const int LED3   = 5;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void loop() {
  int potValue = analogRead(POT);
  long led2Time = map(potValue, 0, 1023, 1000, 2000);

  digitalWrite(LED1, HIGH);
  delay(1100);
  digitalWrite(LED1, LOW);

  digitalWrite(LED2, HIGH);
  delay(led2Time);
  digitalWrite(LED2, LOW);

  digitalWrite(LED3, HIGH);
  delay(1900);
  digitalWrite(LED3, LOW);

}