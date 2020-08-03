/*
  Bilton Box Control Software

  Paolo Brizzolari
*/
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <time.h>

//Threshold Values
int maxtemp=30;
int mintemp=20;
int maxhumidity=50;
int minhumidity=30;

// WIFI
const char* ssid = "<SSID>";
const char* password = "<password>";

//TIME
int timezone = 1;
int dst = 1;

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

/*WEB Server Template Section*/

//This literal creates a page that should be suitable for scraping by prometheus
const char prometheus[] PROGMEM = R"rawliteral(
  fan_speed %FANSPEED% %TIME%
  temperature %TEMPERATURE% %TIME%
  humidity %HUMIDITY% %TIME%
)rawliteral";

//This literal creates a page that is human readable.
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 Biltong Maker</h2>
  <p>
    <i class="fas fa-wind" style="color:#00add6;"></i> 
    <span class="dht-labels">Fan Speed</span>
    <span id="fanspeed">%FANSPEED%</span>
    <sup class="units">RPM</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 2000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 2000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("fanspeed").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/fanspeed  ", true);
  xhttp.send();
}, 2000 ) ;
</script>
</html>)rawliteral";
/*---------------------------*/

//WEB Server Setup
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with DHT values
String processor(const String& var){
//  Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  else if(var == "FANSPEED"){
    return String(RPM);
  }
  else if(var == "TIME"){
    time_t now = time(nullptr);
    return String(ctime(&now));
  }
  return String();
}

void setupWebServer(){
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/prometheus", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", prometheus, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/fanspeed", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(RPM).c_str());
  });

  // Start server
  server.begin();
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  // Set up the serial port for debugging purposes
  Serial.begin(115200);

  //Wifi Setup
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  //Start up the DHT Sensor
  dht.begin();

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  time_t now = time(nullptr);
  Serial.println(ctime(&now));

  //Setup the interrupt to read the fan speed
  pinMode(FANREAD, INPUT_PULLUP);
  interruptCounter = 0;
  RPM = 0;
  attachInterrupt(FANREAD, handleInterrupt, FALLING);

  //Setup the fan to the base speed - Defaults to 1/2 speed
  analogWriteRange(fanrange);
  analogWrite(FANCONTROL, fanlevel);

  setupWebServer();
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
