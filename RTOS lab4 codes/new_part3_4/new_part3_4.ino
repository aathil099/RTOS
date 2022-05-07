//EN20416362 part3_4
#include <stdlib.h>

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static const int led_pin = LED_BUILTIN;
static int led_delay;
static const uint8_t buf_len = 20;

void setup() {
  Serial.begin(115200); 
  pinMode(led_pin, OUTPUT);
  
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      Serial.println();
      Serial.println("Enter a number for delay (milliseconds)");


  
  xTaskCreatePinnedToCore (Task1code,
                            "Task1",
                            1024,
                            NULL,
                            1,
                            NULL,
                            app_cpu);
  
  xTaskCreatePinnedToCore (Task2code,
                            "Task2",
                            1024,
                            NULL,
                            1,
                            NULL,
                            app_cpu);                           
  
 
  vTaskDelete(NULL);                                  
}

void Task1code( void * parameter ){

  // Blink forever and ever
  while (1) {
    
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    }
}

void Task2code( void * parameter ){

  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, buf_len);
  
  // run forever and ever
  while (1) {
    
        
    // Read characters from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // Update delay variable and reset buffer if we get a newline character
      if (c == '\n') {
        led_delay = atoi(buf);
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        Serial.println("Done!");
        memset(buf, 0, buf_len);
        idx = 0;
      } else {
        
        // Only append if index is not over message limit
        if (idx < buf_len - 1) {
          buf[idx] = c;
          idx++;
        }
      }
    }
  }
}


void loop() {

}
