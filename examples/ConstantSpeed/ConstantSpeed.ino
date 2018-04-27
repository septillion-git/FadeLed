/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2016-08-12
 *  @brief Show difference between constant fade speed and constant fade time
 *  
 *  @details This is an example to show the difference between constant fade 
 *  speed (default) and constant fade time. Both LEDs will just fade up and 
 *  down to halve brightness with the same time set. One with constant fade 
 *  speed, one with constant fade time.
 *  
 *  pin 5: Constant fade speed
 *  This LED will fade with constant fade speed. The time set is the time it
 *  takes between off and full brightness (or vice versa). So when fading
 *  by less then all the steps (255) it's finished in less time. For 
 *  example, fading to halve brightness will also take have the time.
 *  
 *  pin 6: Constant fade time
 *  This LED will fade with constant fade time. The time set is the time it
 *  takes for one fade, no matter the values from/to it fades. For example,
 *  a fade between off and full brightness will take the same time as a fade 
 *  from off to 100. Useful for example if you want to fade a RGB LED from 
 *  one color to another in a set time, no matter the levels of the 
 *  individual colors.
 */

#include <FadeLed.h>

//gives the number of elements in an array
#define elements(x) sizeof(x)/sizeof(x[0])

//make two FadeLed objects for pin 5 (leds[0]) and pin 6 (leds[1])
FadeLed leds[] = {5, 6};

void setup() {
  //set the interval (ms) between LED updates
  //it's the same for all FadeLed objects
  //call BEFORE setTime() otherwise the time calculation is wrong
  //default is 50ms (gives 20 updates a second)
  //FadeLed::setInterval(50);
  
  
  //set the fade time for both to 2 seconds.
  
  //leds[0] is set to constant fade speed (default)
  leds[0].setTime(2000);
  //same as 
  //leds[0].setTime(2000, false);
  
  //leds[1] is set to constant fade time, by setting second parameter to true
  leds[1].setTime(2000, true);
}

void loop() {
  // main function call to update all fadeLeds
  //mussed be called often to make it work
  FadeLed::update();
  
  //Want to do the same for all LEDs so we'll loop over all elements of LEDs
  for(byte i = 0; i < elements(leds); i++){
    //to see if it's done fading we can check .done()
    if(leds[i].done()){
      //.get() will return the current brightness
      //so this will tell us if we are not at 0
      if(leds[i].get()){
        //then we are done fading up, let's fade down again
        leds[i].off();
        //same as
        //leds[i].set(0);
      }
      //or we are at 0
      else{
        //then we're done fading down, let's fade up again to halve brightness
        leds[i].set(50);
      }
    }
  }
}