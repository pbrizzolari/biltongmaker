<h1>Biltong Maker</h1>

This code is meant to run a connected biltong maker that will eventually allow you to view details like fan speed, temperature and humidity and hopefully also allow you to change things like the fan speed.

<h2>Part 1 - Temperature & Humidity Sensor</h2>

I've used the dht22 sensor because that's what I had, but the dht11 sensor should work equally well you will just need to choose the correct type when you initialize it.

I've used the adafruit DHT library, but there are other libraries as well.

The nice thing about this sensor is being able to read the temperature and humidity off of a single data pin which is especially useful if you're constrained on pins.

<h2>Part 2 - Reading the PWM Fan Speed</h2>

Reading the PWM fan requires that you use an interrupt on the read pin to count how many times the hall sensor in the fan is triggered. Once you have that value you have to divide it by two since the sensor triggers twice per revolution, then divide it by the interval that you're reading at before lastly multiplying the value by 60 to get into minutes. One issue I had with reading the fan speed is that this particular board (ESP8266 NodeMCU) doesn't have hardware PWM and so sometimes the fan control messed with reading the fan speed. 