#include "WiFi.h"
#include "HTTPClient.h"
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "0SJWL7WQBOKSDG72";
WiFiClient  client;

 
const char* ssid = "NETGEAR95";
const char* password =  "purpleunicorn212";

//my Domain name with URL path or IP address with path
String serverName = "http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr"; //The Username is given here ""




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


 if(WiFi.status()== WL_CONNECTED){
 
   HTTPClient http;   
   String Get_all_lights = serverName + "/lights/3";  //Details of all lights are qued
     // my Domain name with URL path or IP address with path
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
       
 

  delay(1000);
}
}
