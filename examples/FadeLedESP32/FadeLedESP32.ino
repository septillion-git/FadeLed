/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2019-01-19
 *  @brief Example how to use FadeLed library on an ESP32
 *  
 *  @details This is an example how to use FadeLed library on an ESP32. 
 *  
 *  It will fade LED_BUILTIN up and down with a fade time of 5 seconds. Be sure
 *  to set the right board for the right LED_BUILTIN or change it to a pin
 *  you like (and have a led connected). If the selected board has no
 *  LED_BUILTIN, pin 2 is used. 
 *  
 *  The ledc functionality of the ESP32. You can use every pin you like,
 *  just assign it a unique ledc channel. channel 0 to 15 are available.
 *  The default is to use 10-bit like the ESP8266.
 *  
 *  In contrast to most Arduino's, you assign FadeLed to a ledc channel
 *  rather then a pin. You have to setup ledc and assign the pin to a
 *  channel. This example shows you how to do so.
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
FadeLed led(PwmCh);

void setup(){
  //Setup ledc PWM on a ESP32
  pinMode(LedPin, OUTPUT);
  ledcAttachPin(LedPin, PwmCh);
  ledcSetup(PwmCh, 3000, FADE_LED_PWM_BITS); //3kHz, FADE_LED_PWM_BITS => 10-bit

  //by default FadeLed expects a 10-bit PWM (like the ESP8266) but the full range (1 to 16-bit) is supported
  //just use
  //ledcSetup(PwmCh, pwmFrequency, pwmBits)
  //but don't forget to generate and attach the right gamma table for it! Because the 
  //default is still 10-bit. See .setGammaTable() documentation on how to do that.

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