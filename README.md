# Chandelierium by the Seaside

Code tested with Arduino Uno, 3 IR proximity sensors (Sharp 2Y0A02), and 3 Neopixel WS2811 LED strips (one meter = 60 LEDs each). Uses FastSPILED2 library to drive the LED strips: http://code.google.com/p/fastspi/

The interactivity concept is time travel -- you can travel forward in Rainbow Time by interacting with the sensors. 

The LED strips slowly cycle through a rainbow at low brightness when no one is interacting with the sensors. As you approach a sensor, it increases the brightness of the whole assembly and also pushes "forward" through the rainbow as if you are moving forward into the future. The LED strip associated with the sensor you're interacting with moves forward farther through time, pulling the other two LED strips along with it to a lesser extent. Interacting with multiple sensors at once has an additive effect. Then as you move away from the sensor, the rainbow moves "backwards" through time until it returns to the present time. 

If no one interacts with the whole assembly for 30 minutes, the brightness slowly dims to conserve battery life. 

Fritzing wiring diagram:
![Fritzing wiring diagram](http://i.imgur.com/pWChcWg.png)

Laser cutting: https://vine.co/v/b37gM5mK9ae

Final results!
![Chandelierium by the Seaside](http://i.imgur.com/4ngTnLg.jpg)
