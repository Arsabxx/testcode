const int POT    = A2;
const int LED1   = 13;
const int LED2   = 3;
const int LED3   = 5;

unsigned long previousMillis = 0;     
unsigned long cycleInterval = 6000; 

bool led1On = false;
bool led2On = false;
bool led3On = false;

unsigned long led1Duration = 1100;   
unsigned long led3Duration = 1900;    
unsigned long led2Duration = 1500;  

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= cycleInterval) {
    previousMillis = currentMillis;

    int potValue = analogRead(POT);
    led2Duration = map(potValue, 0, 1023, 1000, 2000);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);

    led1On = true;
    led2On = true;
    led3On = true;
  }

  if (led1On && (currentMillis - previousMillis >= led1Duration)) {
    digitalWrite(LED1, LOW);
    led1On = false;
  }

  if (led2On && (currentMillis - previousMillis >= led2Duration)) {
    digitalWrite(LED2, LOW);
    led2On = false;
  }

  if (led3On && (currentMillis - previousMillis >= led3Duration)) {
    digitalWrite(LED3, LOW);
    led3On = false;
  }
}
