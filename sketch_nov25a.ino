#include <Arduino.h>
#include <Arduino_FreeRTOS.h>  


const int POT    = A2;
const int LED1   = 13;
const int LED2   = 3;
const int LED3   = 5;


volatile TickType_t led2OnTicks = pdMS_TO_TICKS(1500);  
const TickType_t    led1OnTicks = pdMS_TO_TICKS(1100);
const TickType_t    led3OnTicks = pdMS_TO_TICKS(1900);
const TickType_t    cycleTicks  = pdMS_TO_TICKS(6000); 


TaskHandle_t xHandlePot   = NULL;
TaskHandle_t xHandleCycle = NULL;

l
void TaskReadPot(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    int potValue = analogRead(POT);
    uint32_t ms = map(potValue, 0, 1023, 1000, 2000);
    led2OnTicks = pdMS_TO_TICKS(ms);   

    vTaskDelay(pdMS_TO_TICKS(500));   
  }
}


void TaskLED1(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED1, HIGH);
    vTaskDelay(led1OnTicks);           
    digitalWrite(LED1, LOW);
    vTaskDelay(cycleTicks - led1OnTicks);  
  }
}


void TaskLED2(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED2, HIGH);
    vTaskDelay(led2OnTicks);           
    digitalWrite(LED2, LOW);
    vTaskDelay(cycleTicks - led2OnTicks);
  }
}

// 任務：控制 LED3（固定 1.9 秒亮）
void TaskLED3(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(LED3, HIGH);
    vTaskDelay(led3OnTicks);
    digitalWrite(LED3, LOW);
    vTaskDelay(cycleTicks - led3OnTicks);
  }
}


void TaskCycleMaster(void *pvParameters) {
  (void) pvParameters;
  for (;;) {

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);

    vTaskDelay(pdMS_TO_TICKS(2000));

    vTaskDelay(cycleTicks - pdMS_TO_TICKS(2000));  // 剩餘時間等待下一輪
  }
}

void setup() {
  Serial.begin(115200);   

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);

  xTaskCreate(TaskReadPot,    "ReadPot",   256, NULL, 1, &xHandlePot);
  xTaskCreate(TaskLED1,       "LED1",      256, NULL, 2, NULL);
  xTaskCreate(TaskLED2,       "LED2",      256, NULL, 2, NULL);
  xTaskCreate(TaskLED3,       "LED3",      256, NULL, 2, NULL);
  xTaskCreate(TaskCycleMaster,"Cycle",     256, NULL, 3, &xHandleCycle);  // 最高優先

}

void loop() {
}