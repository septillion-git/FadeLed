/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2016-08-15
 *  @brief Example how to use FadeLed with RGB-fading
 *  
 *  @details This is an example how to use FadeLed library when fading RGB LEDs so 
 *  they fade smoothly from one color to another. Here we pick a random 
 *  new color every 10 seconds and LED the RGB led fade to the new color 
 *  in 5 seconds. 
 *  
 *  The RGB LED is connected to pins 9, 10 and 11 for red, green en blue 
 *  respectively. Don't forget to add a current limiting resistor for each 
 *  color.
 *  
 *  Pin A0 is used to get a random seed and needs to be unconnected.
 */

#include <FadeLed.h>

//the RGB LED
FadeLed redLed(9);
FadeLed greenLed(10);
FadeLed blueLed(11);

//used to time
unsigned long millisLast = -1;
const unsigned long Interval = 10000;

void setup() {
  //Set update interval to 10ms
  FadeLed::setInterval(10);
  
  //set all colors to same time and to constant fade time
  //Each color change will take 5 seconds
  redLed.setTime(5000, true);
  greenLed.setTime(5000, true);
  blueLed.setTime(5000, true);
  
  //Give the random a random seed from the noise from the ADC of A0
  randomSeed(analogRead(A0));
}

void loop() {
  //We always need to call this
  FadeLed::update();
  
  //Change to a random color every Interval
  if(millis() - millisLast > Interval){
    //Save time for the next time
    millisLast += Interval;
    
    //set each color to a new random value to create a new random color
    redLed.set(random(0, 256));
    greenLed.set(random(0, 256));
    blueLed.set(random(0, 256));
  }
}