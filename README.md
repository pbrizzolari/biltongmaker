<h1>Biltong Maker</h1>

This code is meant to run a connected biltong maker that will eventually allow you to view details like fan speed, temperature and humidity and hopefully also allow you to change things like the fan speed.

<h2>Part 1 - Temperature & Humidity Sensor</h2>

I've used the dht22 sensor because that's what I had, but the dht11 sensor should work equally well you will just need to choose the correct type when you initialize it.

I've used the adafruit DHT library, but there are other libraries as well.

The nice thing about this sensor is being able to read the temperature and humidity off of a single data pin which is especially useful if you're constrained on pins.