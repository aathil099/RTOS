//EN2041636_Part7

TaskHandle_t TaskA;
TaskHandle_t TaskB;

void setup() {
  Serial.begin(9600); 
  xTaskCreatePinnedToCore (TaskAcode,"TaskA",1024,NULL,0,&TaskA,0);            
  xTaskCreatePinnedToCore(TaskBcode,"TaskB",1024,NULL,1,&TaskB,1);
  vTaskStartScheduler();
}

void TaskAcode( void * parameter ){
  for(;;){
    Serial.print("I am Task A I am Running on CORE_");
    Serial.println(xPortGetCoreID());
    vTaskDelay(500 / portTICK_PERIOD_MS);
  } 
}

void TaskBcode( void * parameter ){
  for(;;){
    Serial.print("I am Task A I am Running on CORE_");
    Serial.println(xPortGetCoreID());
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
  }
}

void loop() {

}
