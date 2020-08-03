Hey folks! We're excited to see you at Make Day Biltong! This repo is still very much work in progress, but we thought we'd add some info here if you want to get set up and start having a look around in the meantime. 

<h1>Event details</h1>

When: Tuesday 4 August, 17:30 - 22:30

Where: [Ijver, Scheepsbouwkade 721033 WB, Buiksloterham, Amsterdam](https://goo.gl/maps/ofaWGNBrzpnEieyA9) (There's a free ferry from Amsterdam Centraal - check out  [Pontweer](https://pontveer.nl/stops/Centraal%20Station/NDSM) for times). 

<h1>Prep</h1>

Here's a list of things you'll need to prep before you can set up your biltong maker. 

- Intsall the [Arduino IDE](https://www.arduino.cc/en/main/software)
- Read the [Quick Start to Nodemcu (ESP8266) on Arduino IDE](https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/)
- Read about [Setting up the DHT11 Sensor](https://randomnerdtutorials.com/complete-guide-for-dht11dht22-humidity-and-temperature-sensor-with-arduino/#:~:text=Open%20your%20Arduino%20IDE%20and,Sensor%E2%80%9D%20in%20the%20search%20box)

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

<h2>Part 3 - Fan Control<h2>

Once you know how fast the fan is moving, the next step is being able to control it. The fan that we're using supports pwm on the 4th pin as shown below. If you're not familiar with PWM it stands for pulse width modulation and basically the way it works is that if we want the fan to run at 80% of max speed then we turn on the signal on the pin for 80% of the time and off for the other 20%. Luckily the ESP8266 supports software PWM so instead of us having to calculate and set the on/off times ourselves, we can simply ask the board to do it for us.

![PWM Pinout Diagram](https://lh3.googleusercontent.com/proxy/r-h8nVbhGZ6Fg-itOAlRXevylxRwdrfFHPfjAOBQx322WMGs2BsKR_D3RN-OnebUr3XsWiT-1daHXMLnssaPjH6-EwPv "PWM Pinout Diagram")

<h2>Part 4 - Connecting to wifi</h2>

As we showed on the slide, the ESP8266 has built in wifi and a printed antenna on the chip pcb. This makes it extremely simple to use, since you just import the library for the wifi and ask it to connect. I've found the wifi to be extremely reliable and it has stayed connected for weeks at a time.