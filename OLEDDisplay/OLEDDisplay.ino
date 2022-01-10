// Define constants
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex GPIO 33 is used to wakeup Microcontrolle

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);


static unsigned int state;


void setup() {
  display.begin(0x3C, true); // Address 0x3C default
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  display.setRotation(1);



  pinMode(33, INPUT); // Naviagation Key
  pinMode(27, INPUT); // Selection Key

  Serial.begin(9600);

  state = 1;

}

void loop() {

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
display.println("Device woke up");
        display.display();
        display.clearDisplay();
}

 



  
