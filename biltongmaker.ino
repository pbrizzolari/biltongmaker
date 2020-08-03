/*
  Bilton Box Control Software

  Paolo Brizzolari
*/
#include <Arduino.h>
#include <DHT.h>

//Threshold Values
int maxtemp=30;
int mintemp=20;
int maxhumidity=50;
int minhumidity=30;

//DHT22 - Temperature Sensor
#define DHTTYPE    DHT22      // DHT 22 (AM2302) Could be swapped for DHT 11
#define DHTREAD    14         //DHT Read Pin
DHT dht(DHTREAD, DHTTYPE);    

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
unsigned long previousMillis = 0;    // will store last time DHT was updated
const long interval = 2000;

void displayDHTDetails(){
  // Read temperature as Celsius (the default)
  float newT = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float newT = dht.readTemperature(true);
  // if temperature read failed, don't change the value
  if (isnan(newT)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    t = newT;
    Serial.println(t);
  }
  // Read Humidity
  float newH = dht.readHumidity();
  // if humidity read failed, don't change h value 
  if (isnan(newH)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    h = newH;
    Serial.println(h);
  }
}

//FAN Measurement (PWM Fan)
#define FANREAD 13
volatile int interruptCounter; //counter use to detect hall sensor in fan
int RPM;                      //variable used to store computed value of fan RPM

//This is the function called by the interrupt.
void ICACHE_RAM_ATTR handleInterrupt() { 
    interruptCounter++;

}

void computeFanSpeed() {
    //interruptCounter counts 2 pulses per revolution of the fan over a one second period.
    //We also need to divide by the interval in seconds so that as the interval changes 
    //it doesn't ruin the calculation.
    int count = interruptCounter;
    interruptCounter = 0;
    RPM =  count/2/(interval/1000) * 60 ;
}

void displayFanSpeed() {
    Serial.print(RPM, DEC);        //Prints the computed fan speed to the serial monitor
    Serial.print(" RPM\r\n");      //Prints " RPM" and a new line to the serial monitor
}

//FAN Control
#define FANCONTROL 15
int fanlevel=20;
int fanmin=10;
int fanmax=30;
int fanrange=30;

void controlFan(){
  //Need a flow meter to detect air flow. Recommended is 3 m/s
  //Humidity should be 30-50%
  //Not using temperature range right now but should be fine between 15 and 35.
  if(h > maxhumidity and fanlevel <= fanmax){
    Serial.println("Increasing Fan Speed to "+String(fanlevel));
    fanlevel = fanlevel+1;
    analogWrite(FANCONTROL, fanlevel);
  }else if(h<minhumidity and fanlevel >= fanmin){
    fanlevel = fanlevel-1;
    Serial.println("Decreasing Fan Speed to "+String(fanlevel));
    analogWrite(FANCONTROL, fanlevel);
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  // Set up the serial port for debugging purposes
  Serial.begin(115200);

  //Start up the DHT Sensor
  dht.begin();

  //Setup the interrupt to read the fan speed
  pinMode(FANREAD, INPUT_PULLUP);
  interruptCounter = 0;
  RPM = 0;
  attachInterrupt(FANREAD, handleInterrupt, FALLING);

  //Setup the fan to the base speed - Defaults to 1/2 speed
  analogWriteRange(fanrange);
  analogWrite(FANCONTROL, fanlevel);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis(); //We check the milliseconds each time so that we only read the sensor every <interval> seconds.
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the values
    previousMillis = currentMillis;

    displayDHTDetails();

    //Calculate and print out the fan speed
    computeFanSpeed();
    displayFanSpeed();

    controlFan();
  }
  yield();
}
