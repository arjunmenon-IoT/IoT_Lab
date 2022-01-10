#include "WiFi.h"
#include "HTTPClient.h"
 
const char* ssid = "NETGEAR95";
const char* password =  "purpleunicorn212";
 int brightness =36;

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr"; //The Username is given here ""


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;


void setup() {

 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {

  int Var = analogRead(brightness);
  //Serial. println(Var/16.12); //Converting the The Max Potentio meter to 254 the required LED Brithness value
 //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) 
  {
 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
   String Get_all_lights = serverName + "/lights";  //Details of all lights are qued
     // Your Domain name with URL path or IP address with path
   http.begin(Get_all_lights.c_str()); // get all the Lights satus
   // Send HTTP GET request
      int httpResponseCode_1 = http.GET();
       if (httpResponseCode_1>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode_1);
        String payload = http.getString();
        Serial.println(payload);
       }
       else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode_1);
      }
       
 
   http.begin("http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr/lights/3/state");
   http.addHeader("Content-Type", "text/json");            
   String Value = "{\"hue\":"+ String(analogRead(brightness)/16.1) + "}";
  Serial. println(Value);
   int httpResponseCode = http.PUT(Value);   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
 
   }
 
 
 }else{
    Serial.println("Error in WiFi connection");
 }
  lastTime = millis();
  delay(1000);
}
}
