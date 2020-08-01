/*
  Bilton Box Control Software

  Paolo Brizzolari
*/
#include <Arduino.h>
#include <DHT.h>

//DHT22 - Temperature Sensor
#define DHTTYPE    DHT22      // DHT 22 (AM2302) Could be swapped for DHT 11
#define DHTREAD 14            //DHT Read Pin
DHT dht(DHTREAD, DHTTYPE);    

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
unsigned long previousMillis = 0;    // will store last time DHT was updated
const long interval = 2000;

// the setup function runs once when you press reset or power the board
void setup() {
  
  // Set up the serial port for debugging purposes
  Serial.begin(115200);

  //Start up the DHT Sensor
  dht.begin();
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis(); //We check the milliseconds each time so that we only read the sensor every <interval> seconds.
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
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
}
