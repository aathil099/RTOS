//EN2041636_Part7_2
#include <WiFi.h>

#define WIFI_SSID "EN20416362"
#define WIFI_PASSWORD "wifi1234"
#define WIFI_TIMEOUT_MS 20000
TaskHandle_t TaskA=NULL;
TaskHandle_t TaskB=NULL;

// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
static String header;
// Auxiliar variables to store the current output state
static String LED_1 = "off";
static String LED_2 = "off";
// Assign output variables to GPIO pins
static const int LED1 = 4;
static const int LED2 = 18;
// Current time
static unsigned long currentTime = millis();
// Previous time
static unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
static const long timeoutTime = 2000;
void TaskBcode( void * parameter );
void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW); 
  xTaskCreatePinnedToCore (TaskAcode,"TaskA",5000,NULL,1,&TaskA,0);
  xTaskCreatePinnedToCore (TaskBcode,"TaskB",50000,NULL,2,&TaskB,1);
  vTaskStartScheduler();             
} 

void TaskAcode( void * parameter ){
  for(;;){
    
     if(WiFi.status()== WL_CONNECTED){
        Serial.println("\nConnected!");
        Serial.println(WiFi.localIP());
        digitalWrite(LED_BUILTIN,HIGH);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        continue;
     }
        Serial.print("Connecting!");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        unsigned long startAttemptTime = millis();
        
        while((WiFi.status() != WL_CONNECTED) && (millis()-startAttemptTime < WIFI_TIMEOUT_MS)){}

      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Not Connected!");
        vTaskDelay(20000 / portTICK_PERIOD_MS);
        continue;
     }
     
  } 
}

void TaskBcode( void * parameter ){
   for(;;){
    WiFiClient client = server.available();   // Listen for incoming clients
 
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              LED_1 = "on";
              digitalWrite(LED1, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              LED_1 = "off";
              digitalWrite(LED1, LOW);
            } else if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("GPIO 18 on");
              LED_2 = "on";
              digitalWrite(LED2, HIGH);
            } else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("GPIO 18 off");
              LED_2 = "off";
              digitalWrite(LED2, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 4 - State " + LED_1 + "</p>");
            // If the output26State is off, it displays the ON button       
            if (LED_1=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 18 - State " + LED_2 + "</p>");
            // If the output27State is off, it displays the ON button       
            if (LED_2=="off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
 
  } 

}
void loop() {

}
