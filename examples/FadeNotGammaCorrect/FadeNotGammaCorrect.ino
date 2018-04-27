/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2016-1-30
 *  @brief Example to show the difference between fading with and without gamma correction.
 *  
 *  @details This is an example to show the difference between fading with gamma correction (default) and fading without. With gamma correction will give you a more natural fading.
 *  
 *  By default a 101 step (0 to 100 for full brightness aka percentage) with a gamma 2,3 is used. If no gamma correction is used the range is the full PWM range. For 8-bit (Arduino Uno, Nano, Pro Mini, Mega etc) that gives a range of 0 to 255.
 *  
 *  pin 5
 *  Will fade up and down with gamma correction (default) in 5 seconds
 *  
 *  pin 6
 *  Will fade up and down together with pin 5 but does it without gamma correction.
 */

#include <FadeLed.h>

//make two FadeLed objects for pin 5 (leds[0]) and pin 6 (leds[1])
FadeLed leds[2] = {5, 6};

//it doesn't need to be an array, you can also make separate named objects
//still all updated with FadeLed::update()

void setup() {
  //set the interval (ms) between led updates
  //it's the same for all FadeLed objects
  //call BEFORE setTime() otherwise the time calculation is wrong
  FadeLed::setInterval(20);
  
  //for led[1] no gamma correction is used
  leds[1].noGammaTable();
  
  //both LEDs will take 5 seconds to fade from 0 to max.
  leds[0].setTime(5000);
  leds[1].setTime(5000);
  
  //We fade it to full brightness
  //so will take 5 seconds
  leds[0].on();
  leds[1].on();
}

void loop() {
  // main function call to update all fadeLeds
  //must be called often to make it work
  FadeLed::update();
  
  //we let both LEDs fade up and down all the time
  //to see if it's done fading we can check .done()
  //Because they fade simultaneously in the same time only led[0] is checked
  if(leds[0].done()){
    //.get() will return the current brightness
    //so this will tell us if we are not at 0
    if(leds[0].get()){
      //then we are done fading up, let's fade down again
      leds[0].off();
      leds[1].off();
    }
    //or we are at 0
    else{
      //then we are done fading down, let's fade up again
      leds[0].on();
      leds[1].on();
    }
  }
}