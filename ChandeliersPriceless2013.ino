
// Arduino code for Chandelierium by the Seaside: This works with 3 IR proximity sensors (Sharp 2Y0A02) and 3 Neopixel WS2811 LED strips. 
/*************************************************************************************/

#include "FastSPI_LED2.h"

#define NUM_LEDS 60 // length of LED strips
#define NUM_STRIPS 3 // number of sensors and strips. if you change this number, you also need to set up pins for the additional strips/sensors later on.

struct CRGB leds[NUM_STRIPS][NUM_LEDS];

//Set up pin numbers for IR sensors.
int sensorPin[NUM_STRIPS] = {A0, A2, A4}; 
// A0 sensor --> pin 5 LED strip. 
// A2 sensor --> pin 7 LED strip. 
// A4 sensor --> pin 9 LED strip.
int dist[NUM_STRIPS] = {0};

// SMOOTHING: Define the number of samples for smoothing the sensor signal. 
const int numReadings = 5;
int index = 0;                  // the index of the current reading
int readings[NUM_STRIPS][numReadings] = {0};  // array to hold the sensor values, for each sensor
int total[NUM_STRIPS] = {0};                  // the running total for each sensor
int average[NUM_STRIPS] = {0};                // the average for each sensor

// "Mood light" color cycling
int colorval = 0; // Variable to hold the changing "mood light" strip color.
const int mooddelay = 1; // Increase this to slow down the color cycling.
int moodcounter = 0; // just a counter...

// Variables for dimming the light when inactive
unsigned long dimtime = 600000; //how long it waits before dimming. 30 minutes in milliseconds = 600000
unsigned long timer = 1000; //just a timer thing....
int dimlevel = 150; // set initial brightness; can go from 1 to 255.

void setup() {
   LEDS.addLeds<WS2811, 5>(leds[0], NUM_LEDS);
   LEDS.addLeds<WS2811, 7>(leds[1], NUM_LEDS);
   LEDS.addLeds<WS2811, 9>(leds[2], NUM_LEDS);
   LEDS.setBrightness(dimlevel);
}


void loop() {
  int i;
  //This loop reads the sensors and smooths the values over time.
  for(i=0; i<NUM_STRIPS; i++){
    // read the value from the sensor and compress ends of scale:
    analogRead(sensorPin[i]); // something about reading adjacent pins too fast causing interference?? not sure if reading twice is necessary.
    delay(10);
    dist[i] = analogRead(sensorPin[i]);    
    //Smooth the value over time! subtract the last reading:
    total[i]= total[i] - readings[i][index];         
    readings[i][index] = dist[i];   // put sensor reading into array:  
    total[i]= total[i] + readings[i][index];   // add the reading to the total:
    average[i] = total[i] / numReadings;   // calculate the average:  
    if(average[i]<200) average[i]=200;  
    dist[i] = map(average[i], 200, 500, 1, 30);   // Rescale the signal to a reasonable range
    if(dist[i] < 1) dist[i]=1; //shouldn't be necessary, but it would be really bad to go out of range.
  }

  index = index + 1;          // advance to the next position in the array:   
  if(index >= numReadings) index = 0;   // wrap around
 
  int sumdist = 0; // add up all the sensor distances to decide how much to progress through the rainbow
  for(i=0; i<NUM_STRIPS; i++) sumdist += dist[i];
  
  //Light up each strip, pushing farther through the rainbow for higher sensor values 
  //and adding "bonus distance" to each sphere for its own sensor reading:  
  for(i=0; i<NUM_STRIPS; i++) rainbow(leds[i], (colorval + sumdist + 2*dist[i]) % 384);
  LEDS.show();   // write all the pixels out

  //Mood light cycling with variable delay.
  if(moodcounter < mooddelay) moodcounter++;
  else 
    {
      colorval++;
      moodcounter = 0;
    }
    if(colorval>384) colorval=0; //loop around at the end of the rainbow

   if(sumdist > 4 && dimlevel < 250) // dimmer setting: ramp up the brightness to 250 if someone is playing with it
     {
       dimlevel += 2;
       timer = millis(); //reset the timer 
     }
   if(millis() > (timer + dimtime) && dimlevel > 50) 
    {
      dimlevel -= 1; //if no one has played in a while, turn down slowly.
    }
   LEDS.setBrightness(dimlevel); 
}

 //Fill the strip with 1/3 of a rainbow, starting from color c
void rainbow(struct CRGB *leds, uint32_t c) {
  uint16_t i;
    for (i=0; i < NUM_LEDS; i++) {
      leds[i] = RGBWheel(((i * 128 / NUM_LEDS) + c) % 384); // 128 = 384/3 = 1/3 of the rainbow.
    }
}

/* RGB Color Wheel Helper function */
//Input a value 0 to 384 to get a color value. 
CRGB RGBWheel(uint16_t WheelPos) {
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = (127 - WheelPos % 128); // red down
      g = (WheelPos % 128);       // green up
      b = 0;                    // blue off
      break;
    case 1:
      g = (127 - WheelPos % 128); // green down
      b = (WheelPos % 128);       // blue up
      r = 0;                    // red off
      break;
    case 2:
      b = (127 - WheelPos % 128); // blue down
      r = (WheelPos % 128);       // red up
      g = 0;                    // green off
      break;
  }
  return(CRGB(g, r, b));   //My LED strips are wired in a g,r,b order for some reason. 
}

