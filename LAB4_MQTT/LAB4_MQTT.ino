#include "WiFi.h"
#include "HTTPClient.h"
#include "ThingSpeak.h"
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "0SJWL7WQBOKSDG72";
WiFiClient  client;

#include "PubSubClient.h" //Knolleary
#include  <WiFi.h> //ESP8266WiFi.h

char ssid[] = "NETGEAR95";
char pass[] = "purpleunicorn212";

const char mqttUserName[] = "FBo5NhcyEywZJQM8FjcQORQ"; 
const char clientID[] = "FBo5NhcyEywZJQM8FjcQORQ";
const char mqttPass[] = "hPVaikuEIPZq87Vh8TEJr6q1";
int channelID =1615114;
#ifdef ESP8266BOARD
  #include <ESP8266WiFi.h>
  const char* PROGMEM thingspeak_cert_thumbprint = "271892dda426c30709b97ae6c521b95b48f716e1";
#else
  #include <WiFi.h>
  const char * PROGMEM thingspeak_ca_cert = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n" \
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
  "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n" \
  "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n" \
  "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n" \
  "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n" \
  "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n" \
  "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n" \
  "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n" \
  "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n" \
  "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n" \
  "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n" \
  "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n" \
  "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n" \
  "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n" \
  "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n" \
  "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n" \
  "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n" \
  "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n" \
  "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n" \
  "+OkuE6N36B9K\n" \
  "-----END CERTIFICATE-----\n";
#endif

#ifdef USESECUREMQTT
  #include <WiFiClientSecure.h>
  #define mqttPort 8883
  WiFiClientSecure client; 
#else
  #define mqttPort 1883
  WiFiClient client;
#endif



const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );

// Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ){
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  mqttClient.subscribe(myTopic.c_str());
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}


// Connect to WiFi.
void connectWifi()
{
  Serial.print( "Connecting to Wi-Fi..." );
  // Loop until WiFi connection is successful
  #ifdef ESP8266BOARD
    while ( WiFi.waitForConnectResult() != WL_CONNECTED ) {
  #else
    while ( WiFi.status() != WL_CONNECTED ) {
  #endif
    WiFi.begin( ssid, pass );
    delay( connectionDelay*1000 );
    Serial.print( WiFi.status() ); 
  }
  Serial.println( "Connected to Wi-Fi." );
}

// Connect to MQTT server.
void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}
// Define constants
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex GPIO 33 is used to wakeup Microcontrolle

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //OLED display
#include <Adafruit_SH110X.h>//OLED diaplay
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire); //OLED Display
 
const char* ssid = "NETGEAR95";
const char* password =  "purpleunicorn212";
 int color =36; //PotentioMeter connected

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr"; //The Username is given here ""


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;

unsigned long timerDelay = 10000;
 int temt6000 = 34; //temt6000 sensor conncted


void setup() {

  // Delay to allow serial monitor to come up.
  delay(3000);
  // Connect to Wi-Fi network.
  connectWifi();
  // Configure the MQTT client
  mqttClient.setServer( server, mqttPort ); 
  // Set the MQTT message handler function.
  mqttClient.setCallback( mqttSubscriptionCallback );
  // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );
  // Use secure MQTT connections if defined.
  #ifdef USESECUREMQTT
    // Handle functionality differences of WiFiClientSecure library for different boards.
    #ifdef ESP8266BOARD
      client.setFingerprint(thingspeak_cert_thumbprint);
    #else
      client.setCACert(thingspeak_ca_cert);
    #endif
  #endif

 
  Serial.begin(115200);
  ThingSpeak.begin(client);  // Initialize ThingSpeak

display.begin(0x3C, true); // Address 0x3C default
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  display.setRotation(1); //Initilise the OLED DISPLAY
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
int sensorValue = analogRead(36);
 sensorValue=sensorValue *254;
  sensorValue=sensorValue /4095; // convert the pot vaue into 0 to 254

// Reading  Temt6000 valur value
int temt6000 = analogRead(34); //Divide by maximum value;

 
 
 
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
       
 
   http.begin("http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr/lights/5/state");
   http.addHeader("Content-Type", "text/json");            
   String Value = "{\"sat\":"+ String(sensorValue) + "}";  //control the RGB value with Potentiometer
  Serial. println(sensorValue);
   int httpResponseCode = http.PUT(Value);   
 
   if(httpResponseCode>0){
 
    String response = http.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);     


  // When The luminosity is less than britness increases Min brigntess
          
           while (temt6000<200) {   //if the illuminosity us less than 200 turn the britness to max 254
                                    http.begin("http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr/lights/5/state");
                                    http.addHeader("Content-Type", "text/json");            
                                    String Value_1 = "{\"bri\":"+ String(254) + "}";  //control the RGB value with Potentiometer
                                    int httpResponseCode_2 = http.PUT(Value_1);   
 
                                        if(httpResponseCode_2>0)
                                        {
                                          String response_2 = http.getString();
                                          Serial.println(httpResponseCode_2);
                                          Serial.println(response_2);   
                                        }

                            }
             while (temt6000<900) { //if the illuminosity us higher than 900 turn the britness to min median 125
                                    http.begin("http://192.168.1.149/api/Pk1DiCvNoe-km4GFUPTlFjfZlO2A1JX8P1au-7Cr/lights/5/state");
                                    http.addHeader("Content-Type", "text/json");            
                                    String Value_2 = "{\"bri\":"+ String(125) + "}";  //control the RGB value with Potentiometer
                                    int httpResponseCode_3 = http.PUT(Value_2);   
 
                                        if(httpResponseCode_3>0)
                                        {
                                          String response_3 = http.getString();
                                          Serial.println(httpResponseCode_3);
                                          Serial.println(response_3);   
                                        }

                            }


                            //The Parameters are displayed in OLED 
 display.setTextSize(1);
 display.setTextColor(SH110X_WHITE);
 display.setCursor(0, 0);
display.println("Light Group :5");
display.println("PoT value :" +String(sensorValue));
display.println("Britness : " +String(temt6000));

display.display();
display.clearDisplay();
 
   }else
   {
 
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
