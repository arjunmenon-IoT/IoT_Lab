/*

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And tx_humidity is the data to be sent, in this example just a byte incremented every second. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>



BLECharacteristic *pCharacteristic;
bool deviceConnected = false;


float luminosity = 0;                      //luminosity variable initilisation.



#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"       // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Control Recieved: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();

        // Do stuff based on the command received from the app
        if (rxValue.find("ON") != -1) { 
          Serial.print("Turning ON!");     // If the command ON recives from andriod app the led swtich ON
          digitalWrite(LED, HIGH);
        }
        else if (rxValue.find("OFF") != -1) {
          Serial.print("Turning OFF!");     // If the command OFF recives from andriod app the led swtich OFF
          digitalWrite(LED, LOW);
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  dht.begin();            //DHT22 Sensor begin funtion.
  pinMode(LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("Group NO: 21/FINAL"); // Give it a name

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  if (deviceConnected) {      // When ESP32 connected with Andriod UART App the function begins.
  
    tx_luminosity  = dht.readHumidity();     //Only Humidity is sensed here in this example and loaded to the variable

    
   
    char txString[8]; // make sure this is big enuffz
    dtostrf(tx_luminosity, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buf_humdity_humdityfer
    

  
    String tx_luminosity =  "luminosity " + String(txString) + " ";   //concating the string  with char
    char buf_luminosity[50];                                      
    tx_humdty.toCharArray(buf_luminosity, 50);                    // String to char again :) beacuse 
    pCharacteristic->setValue(buf_luminosity);                    // set value only takes char value ;)
    
    pCharacteristic->notify();                                 // Send the value to the app!
    Serial.print("*** luminosity: ");
    Serial.print(txString);
    Serial.println(" ***");

  }
  delay(1000);
}
