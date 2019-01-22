/**
 *  @file
 *  @Author Septillion (https://github.com/septillion-git)
 *  @date 2019-01-22
 *  @brief Example how to use FadeLed library on an ESP32 in 16-bit mode
 *  
 *  @details This is an example how to use FadeLed library on an ESP32. 
 *  
 *  It does the same as the FadeLedESP32 example only changes the PWM
 *  resolution to 16-bit instead of the default 10-bit. If you want to
 *  use 16-bit (or anything other than the default 10-bit) you
 *  have to link a correct gamma table yourself. This example shows how
 *  you can use the default gamma = 2,3 tables supplied.
 *  
 *  The supplied tables are generates with the supplied GammaTable Python
 *  script but you are free to generate any type of table you like! See for
 *  example the SineFade-example.
 *  
 */

#include <FadeLed.h>

//Use the LED_BUILTIN if it exists, if not pin 2 is used
#if defined(LED_BUILTIN)
  byte LedPin = LED_BUILTIN; //Pin to fade
#else
  byte LedPin = 2; //Pin to fade
#endif
byte PwmCh = 0; //ledc channel to use for that

//make a FadeLed object
//on a ESP you assign FadeLed to a ledc channel rather than a pin
//Second argument is the gamma table. To use the default tables
//just use FadeLedGammaTable[x] with [x] the number of bits for 
//this ledc channel, e.g. FadeLedGammaTable16 for 16-bit.
//Third argument is the biggest step in that table
//The default tables are 0 to 100, hence 100.
FadeLed led(PwmCh, FadeLedGammaTable16, 100);

void setup(){
  //Setup ledc PWM on a ESP32
  pinMode(LedPin, OUTPUT);
  ledcAttachPin(LedPin, PwmCh);
  ledcSetup(PwmCh, 3000, 16); //3kHz, 16-bit

  //set the interval (ms) between LED updates
  //it's the same for all FadeLed objects
  //call BEFORE setTime() otherwise the time calculation is wrong
  //default is 50ms (gives 20 updates a second)
  //FadeLed::setInterval(50);
  
  //Set a fade time of 5 seconds
  led.setTime(5000);

  //and make it fade to full brightness
  led.on();
}

void loop(){
  //main function call to update all FadeLed objects
  //must be called often to make it work
  FadeLed::update();
  
  //we let led fade up and down all the time
  //to see if it's done fading we can check .done()
  if(led.done()){
    //.get() will return the current brightness
    //so this will tell us if we are not at 0
    if(led.get()){
      //then we are done fading up, let's fade down again
      led.off();
    }
    //or we are at 0
    else{
      //then we are done fading down, let's fade up again
      led.on();
    }
  }
}