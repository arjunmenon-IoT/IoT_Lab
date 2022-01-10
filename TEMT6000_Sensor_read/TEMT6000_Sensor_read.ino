// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int temt6000 = 34;

// variable for storing the potentiometer value
int value = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading  Temt6000 valur value
  value = analogRead(temt6000); //Divide by maximum value;
 // value =(value/)*100;// Square to make response more obvious
  Serial.println(value); 
  delay(100); //in 100 ms
}
