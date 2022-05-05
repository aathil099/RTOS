//EN20416362 part3_3

TaskHandle_t Task1;
TaskHandle_t Task2;

const int led_1 = 4;
const int led_2 = 5;


void setup() {
  Serial.begin(9600); 
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  xTaskCreatePinnedToCore (Task1code,"Task1",10000,NULL,1,&Task1,0);            
  xTaskCreatePinnedToCore(Task2code,"Task2",10000,NULL,1,&Task2,1);
  vTaskStartScheduler();
}

void Task1code( void * parameter ){
  for(;;){
    Serial.print("Task1 is running on core ");
    Serial.println(xPortGetCoreID());
    
    digitalWrite(led_1, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_1, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
    
  } 
}

void Task2code( void * parameter ){
  for(;;){
    Serial.print("Task2 is running on core ");
    Serial.println(xPortGetCoreID());
    
    digitalWrite(led_2, HIGH);
    vTaskDelay(300/portTICK_PERIOD_MS);
    digitalWrite(led_2, LOW);
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}

void loop() {
  
}
