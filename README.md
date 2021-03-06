Hey folks! We're excited to see you at Make Day Biltong! This repo is still very much work in progress, but we thought we'd add some info here if you want to get set up and start having a look around in the meantime. 

<h1>Event details</h1>

When: Tuesday 4 August, 17:30 - 22:30

Where: [Ijver, Scheepsbouwkade 721033 WB, Buiksloterham, Amsterdam](https://goo.gl/maps/ofaWGNBrzpnEieyA9) (There's a free ferry from Amsterdam Centraal - check out  [Pontweer](https://pontveer.nl/stops/Centraal%20Station/NDSM) for times). 

<h1>Bill of Materials</h1>

[Plastic Storage Box](https://www.gamma.nl/assortiment/opbergbox-click-45l-incl-deksel/p/B603866)

[Fan](https://www.amazon.nl/ARCTIC-standaard-ventilator-behuizing-configuratie/dp/B002KTVFTE/ref=sr_1_8?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=4+pin+140mm+fan&qid=1621331012&sr=8-8)

[Temperature and humidity sensor](https://www.amazon.nl/Temperatuur-Vochtigheid-Digitale-Raspberry-Huishoudapparaat/dp/B07TWBW21H/ref=sr_1_17?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=dht22&qid=1621331030&sr=8-17)

[ESP8266](https://www.amazon.nl/AZDelivery-NodeMCU-Lolin-V3-Development/dp/B06Y1ZPNMS/ref=sr_1_6?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=esp8266&qid=1621331177&sr=8-6)

[12v 1A Power Supply](https://www.amazon.nl/Dericam-Adapter-security-100V-240V-European/dp/B07PHKHWQG/ref=sr_1_6?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=12v+power+supply&qid=1621331270&refinements=p_36%3A17170621031&rnid=16365235031&s=electronics&sr=1-6)
 
[Optional Buck Converter](https://www.amazon.nl/HER-XD-45-Mini-360-voeding-LM2596/dp/B076M79TVS/ref=sr_1_1?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=mini+360&qid=1621331327&sr=8-1)
(Lets you share the power supply between the board and the fan)

[Mesh Cover for the holes in the biltong maker](https://www.amazon.nl/Fashion-Cuttable-Stoffilter-Dustproof-Computer/dp/B08D6D2TR6/ref=sr_1_25?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=pc+fan+mesh&qid=1621331380&sr=8-25)
(I don’t like the idea of flies getting in)

<h1>Building the circuit</h1>
Build the circuit as shown below to get the wiring in the correct place for the code as it is written now.

![Circuit Design](CircuitDesign.png "Circuit Design")

<h1>Prep</h1>

Here's a list of things you'll need to prep before you can set up your biltong maker. 

- Intsall the [Arduino IDE](https://www.arduino.cc/en/main/software)
- Read the [Quick Start to Nodemcu (ESP8266) on Arduino IDE](https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/)
- Read about [Setting up the DHT11 Sensor](https://randomnerdtutorials.com/complete-guide-for-dht11dht22-humidity-and-temperature-sensor-with-arduino/#:~:text=Open%20your%20Arduino%20IDE%20and,Sensor%E2%80%9D%20in%20the%20search%20box)

Important: if you want to use the ESP8266 WEB Server in this example you will need to downloand the following two libraries. These libraries are not available in the library manager.

<h2>Installing the ESPAsyncWebServer library</h2>
The ESPAsyncWebServer library is not available to install in the Arduino IDE Library Manager. So, you need to install it manually.

Follow the next steps to install the ESPAsyncWebServer library:

https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

Unzip the .zip folder and you should get ESPAsyncWebServer-master folder
Rename your folder from ESPAsyncWebServer-master to ESPAsyncWebServer
Move the ESPAsyncWebServer folder to your Arduino IDE installation libraries folder

<h2>Installing the ESPAsync TCP Library</h2>
The ESPAsyncWebServer library requires the ESPAsyncTCP library to work. Follow the next steps to install that library:

https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

Unzip the .zip folder and you should get ESPAsyncTCP-master folder
Rename your folder from ESPAsyncTCP-master to ESPAsyncTCP
Move the ESPAsyncTCP folder to your Arduino IDE installation libraries folder
Finally, re-open your Arduino IDE

You may also need [the driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) for the usb-uart

<h1> What will be available for you on the evening </h1>

- Wi-Fi module based on ESP8266
- DHT11 digital temperature and humidity sensor
- Fans, biltong boxes, dowel rods, wiring etc
- Soldering stations, Dremel
- A guide on how to set up and someone who's done it before

<h1>Setting up your Biltong Maker</h1>

This code is meant to run a connected biltong maker that will eventually allow you to view details like fan speed, temperature and humidity and hopefully also allow you to change things like the fan speed.

<h2>Part 1 - Temperature & Humidity Sensor</h2>

I've used the dht22 sensor because that's what I had, but the dht11 sensor should work equally well you will just need to choose the correct type when you initialize it.

I've used the adafruit DHT library, but there are other libraries as well.

The nice thing about this sensor is being able to read the temperature and humidity off of a single data pin which is especially useful if you're constrained on pins.

<h2>Part 2 - Reading the PWM Fan Speed</h2>

Reading the PWM fan requires that you use an interrupt on the read pin to count how many times the hall sensor in the fan is triggered. Once you have that value you have to divide it by two since the sensor triggers twice per revolution, then divide it by the interval that you're reading at before lastly multiplying the value by 60 to get into minutes. One issue I had with reading the fan speed is that this particular board (ESP8266 NodeMCU) doesn't have hardware PWM and so sometimes the fan control messed with reading the fan speed. 

<h2>Part 3 - Fan Control</h2>

Once you know how fast the fan is moving, the next step is being able to control it. The fan that we're using supports pwm on the 4th pin as shown below. If you're not familiar with PWM it stands for pulse width modulation and basically the way it works is that if we want the fan to run at 80% of max speed then we turn on the signal on the pin for 80% of the time and off for the other 20%. Luckily the ESP8266 supports software PWM so instead of us having to calculate and set the on/off times ourselves, we can simply ask the board to do it for us.

![PWM Pinout Diagram](https://lh3.googleusercontent.com/proxy/r-h8nVbhGZ6Fg-itOAlRXevylxRwdrfFHPfjAOBQx322WMGs2BsKR_D3RN-OnebUr3XsWiT-1daHXMLnssaPjH6-EwPv "PWM Pinout Diagram")

<h2>Part 4 - Connecting to wifi</h2>

As we showed on the slide, the ESP8266 has built in wifi and a printed antenna on the chip pcb. This makes it extremely simple to use, since you just import the library for the wifi and ask it to connect. I've found the wifi to be extremely reliable and it has stayed connected for weeks at a time.

<h2>Part 5 - Setting Up a WebServer</h2>

This part looks like a lot of code, but really most of it is just the template for our web page. The Web server library supports templated web pages which makes it nice and easy to insert our values that we read from the sensors.

<h2>ESP8266 Pinout</h2>

![ESP8266 Pinout Diagram](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266-NodeMCU-kit-12-E-pinout-gpio-pin.png?w=817&ssl=1 "ESP8266 Pinout Diagram")


<h2>Sources Used</h2>

The article below details which pins are freely available and which might cause issues. I found it quite useful
[ESP Pin Usage](https://randomnerdtutorials.com/esp8266-pwm-arduino-ide/)

DHT & Web Server - [Random Nerd Tutorials](https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/)

PWM- [Random Nerd Tutorials](https://randomnerdtutorials.com/esp8266-pwm-arduino-ide/)

<h2>Further Ideas</h2>

Currently I am powering the ESP off of a standard USB charger which works quite well. It is also possible though to use a DC/DC converter to step down the voltage from the 12v Adaptor used for the fan to about 7v which is acceptable for the ESP. That will mean that you only need 1 power adapter.

Many biltong makers include a lamp as a heat source, I haven't really found it necessary here, but it's another quite simple addition that can be done by adding a relay which controls a heat lamp meant for lizard terrariums.

I've included a page on the web server which was to provide the data to prometheus for scraping. The idea with this was to be able to store the data and look back once a really great batch of biltong was made in order to be able to replicate the conditions (as much as possible).