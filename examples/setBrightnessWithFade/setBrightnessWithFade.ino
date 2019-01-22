/**
 *  @file
 *  @Author Septillion (https://github.com/septillion-git)
 *  @date 2016-08-17
 *  @brief Example how to use FadeLed to set the brightness of a LED with a button
 *  
 *  @details This example requires the Bounce2 library (https://github.com/thomasfredericks/Bounce2)
 *  
 *  This shows how you can make a LED which you can fade up and down to set the desired brightness. The brightness will keep fading up and down as long as the button is pressed and will stop at the current brightness once the button is released. Fading will start in opposite direction is the button is pressed again.
 *  
 *  Pin 5 is used for the LED
 *  
 *  Pin 4 is used for a button. Connect a push button between this pin and GND.
 *  
 */

#include <FadeLed.h>
#include <Bounce2.h>

//the RGB LED
FadeLed led(5);

//Which way did we fade last?
bool wasFadingUp = false;

//the button
const byte ButtonPin = 4;
Bounce button;

void setup() {
  //Set update interval to 10ms
  FadeLed::setInterval(10);
  
  //set fade time to 4 seconds
  led.setTime(4000);

  //setup the button
  button.attach(ButtonPin, INPUT_PULLUP);
}

void loop() {
  //We always need to call this
  FadeLed::update();
  
  //Now we update the button
  button.update();
  
  //if the button became pressed we start to fade
  if(button.fell()){
    //if we stopped the last time fading down we resume fading up
    if(!wasFadingUp){
      led.on();
    }
    //otherwise we start fading down
    else{
      led.off();
    }
  }
  
  //While the button is pressed we just keep fading between off and on
  if(!button.read()){
    if(led.done()){
      if(led.get()){
        led.off();
      }
      else{
        led.on();
      }
    }
    //Save which way we fade
    wasFadingUp = led.rising();
  }
  
  //Once the buttons became released, we stop the fading at the current brightness
  if(button.rose()){
    led.stop();
  }
}