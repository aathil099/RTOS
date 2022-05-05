//EN20416362 part3_4

static const int led_pin = LED_BUILTIN;
static long int delay_arg;

void setup() {
  Serial.begin(115200); 
  pinMode(led_pin, OUTPUT);
  
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      Serial.println();
      Serial.println("Enter a number for delay (milliseconds)");

      // Wait for input from Serial
      while (Serial.available() <= 0);

      // Read integer value
      delay_arg = Serial.parseInt();
      Serial.print("Sending: ");
      Serial.println(delay_arg);
  
  xTaskCreate (Task1code,"Task1",10000,(void *)&delay_arg,2,NULL); 
  
  Serial.println("Done!");
  vTaskDelete(NULL);                                  
}

void Task1code( void * parameter ){
 int num = *(int *)parameter;

  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  // Configure the LED pin
  pinMode(led_pin, OUTPUT);

  // Blink forever and ever
  while (1) {
    
        digitalWrite(led_pin, HIGH);
        vTaskDelay(num / portTICK_PERIOD_MS);
        digitalWrite(led_pin, LOW);
        vTaskDelay(num / portTICK_PERIOD_MS);
    }
}


void loop() {

}
