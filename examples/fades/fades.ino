/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2016-08-11
 *  @brief Example how to use FadeLed library
 *  
 *  This is an example how to use FadeLed library. It will fade two leds
 *  in two different ways on two different pins.
 *  
 *  pin 5
 *  Will fade up and down all the time but only up to halve brightness.
 *  It will do this in 5 seconds
 *  
 *  pin 6
 *  Will fade up/down every 5 seconds. It will begin turned on at
 *  full brightness. If will fade up in 4 seconds and down in 2 seconds.
 *  A led at pin 12 and pin 13 will indicate if the led on pin 6 is 
 *  fading down or up correspondingly.
 */

#include <FadeLed.h>

//make two FadeLed objecs for pin 5 (leds[0]) and pin 6 (leds[1])
FadeLed leds[2] = {5, 6};

//it doesn't need to be an array, you can also make seperate named objects
//still all updated with FadeLed::update()

//we turn this pin on will leds[1] is fading down
const byte FadeDownPin = 12;
//we turn this pin on will leds[1] is fading up
const byte FadeUpPin = 13;

unsigned long millisLast;

void setup() {
  //set pin mode of the fade pins
  pinMode(FadeDownPin, OUTPUT);
  pinMode(FadeUpPin, OUTPUT);
  
  //set the interval (ms) between led updates
  //it's the same for all FadeLed objects
  //call BEFORE setTime() otherwise the time calculation is wrong
  //default is 50ms (gives 20 updates a second)
  //FadeLed::setInterval(50);
  
  //led[0] will take 10 seconds to fade from 0 to max
  //takes less time for smaller difference
  leds[0].setTime(10000);
  
  //We fade it to halve brightness
  //so will take 5 seconds
  leds[0].set(127);
  
  //leds[1] will use the default fade time of 2 seconds
  //leds[1].setTime(2000);
  
  //We turn this led fully on directly (without fading) now
  leds[1].beginOn();
  //is the same as
  //leds[1].begin(255);
  
  //and we let it fade to off for now
  leds[1].off();
  //same as
  //leds[1].set(0);
}

void loop() {
  // main function call to update all fadeLeds
  //mussed be called often to make it work
  FadeLed::update();
  
  //we let leds[0] fade up and down all the time
  //up to halve brightness (127)
  //to see if it's done fading we can check .done()
  if(leds[0].done()){
    //.get() will return the current brightness
    //so this will tell us if we are not at 0
    if(leds[0].get()){
      //then we are done fading up, let's fade down again
      leds[0].off();
    }
    //or we are at 0
    else{
      //then we are done fading down, let's fade up again
      leds[0].set(127);
    }
  }
  
  //We let leds[1] fade up or down every 5 seconds
  //we fade up in 4 seconds and down in 2 seconds.
  
  //change every 5 seconds
  //see: Blink without delay
  if(millis() - millisLast > 5000){
    millisLast = millis();
    
    //if led is not off (.get() returns something bigger then 0)
    if(leds[1].get()){
      //we set the fading time to 2 seconds
      leds[1].setTime(2000);
      //and start to fade off
      leds[1].off();
    }
    //if the led is off
    else{
      //we set the fading time to 4 seconds
      leds[1].setTime(4000);
      //and start to fade to full brightness
      leds[1].on();
      //same as
      //leds[1].set(255);
    }
  }
  
  //.falling() will be true/HIGH while the led is fading down
  //.rising() will be true/HIGH while the led is fading up
  //we can use this to trigger actions like turning on another led while fading up/down
  digitalWrite(FadeDownPin, leds[1].falling());
  digitalWrite(FadeUpPin, leds[1].rising());
  
  
}