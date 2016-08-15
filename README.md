# FadeLed
Makes fading leds on an Arduino easy.
By Timo Engelgeer (Septillion)

##What does it do?
Fading is easy right? But it can quickly become quite some code to do so. FadeLed does all the heavy lifting for you. It can fade every led on a *hardware* PWM pin (capable of `analogWrite()`). Just make a FadeLed object for it, set the fade time and just set the brightness to fade to. You can let it fade to and from each brightness you like!

FadeLed can fade in two modes, constant fade speed (default) and constant fade time. The mode can be selected for each led.

####Constant fade speed
This is the default mode. A led will fade with a constant speed. The time you set is the time of a full fade from off to full brightness or vice versa. If the fade is not full scale, for example to/from halve brightness, it will take less time. In the case of halve brightness it will take halve the time.

####Constant fade time
Each fade will now take the same amount of time. No matter if you fade the full scale or just just by 10 steps, it will take the same time. 

##Download and install
Laterst release: **[v1.2.0](https://github.com/septillion-git/FadeLed/archive/v1.2.0.zip)**

1. Download the latest release.
2. Extract it to the `libraries` folder **inside** your Sketchbook. Default is `[user]\Arduino\libraries`. 
3. Rename the folder to `FadeLed` (remove version number).
4. Restart the Arduino IDE if you had it open.
5. Done!

##Usage
###FadeLed-object
Using FadeLed is simple. Just make a FadeLed object for each led you want to fade like

```C++
FadeLed statusLed(5); //Fading status led on pin 5
FadeLed anotherLed(6); //Other fading led on pin 6
```

It's also possible to make an array of multiple FadeLed-objects which can make it easy to loop over each.

```C++
FadeLed myLeds[] = {5, 6}; //creates two FadeLed objects: leds[0] and leds[2]
```

###void FadeLed::update()
In order to update the fading of all FadeLed objects you just have to call `FadeLed::update()` frequently. So it's best to have a non-blocking loop and call it like
```C++
void loop(){
  FadeLed::update(); //updates all FadeLed objects
  
  /*
  Do all the other things you want to do
  */
}
```

###void .setTime(unsigned long time)
Calling this function will set the time a fade needs to take. By default it will set the time a full fade should take (constant fade speed). But by entering `true` as second parameter you can change that to the time each fade should take (constant fade time). You can change the fade time anytime you like! The time is set in **milliseconds**

```C++
statusLed.setTime(5000); //makes the statusLed fade with constant fade speed. Full fade will take 5 seconds
anotherLed.setTime(1000, true); //makes this led fade with constant fade time. Each fade now takes 1 second
```

###void .set(byte value)
The most common function of the library. Simply sets the brightness to fade to.

```C++
statusLed.set(127); //fades the statusLed to halve brightness
```

There are also the shortcuts `.on()` and `.off()` to simply fade to full on or full off respectifly.

##More methodes
Other usefull methodes of the library include `.on()`, `.off()`, `.done()`, `.get()`, `.rising()`, `.falling()` and `FadeLed::setInterval()`. For documentation of all the methodes, see the full documentation.

##Full documentation
Full documentation of all the methodes of this library can be found inside the library located in `FadeLed\doc`. Just open `FadeLed\doc\index.html` to see all methodes of FadeLed. 

This documentation is powered by [Doxygen](http://www.doxygen.org/) and thus fully extracted from the source files. 

##FAQ

###My led doesn't fade nice/all the time!
Check to see if `FadeLed::update()` is called regularly. So you should **not** use blocking code! The `loop()` should run freely. The biggest example of blocking code is the `delay()` function. (See [Blink without delay](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) how to fix that.) But also other functions can block like `Serial.readBytesUntil()`, `Serial.parseInt()` or other functions that just wait until something happens.

###Nothing happens!
Are you calling `FadeLed::update()` freqently? Have you used a PWM (capable of `analogWrite()`) pin? 

If you do, are you using another library (or code) that uses a timer? For example `Servo`. This will block the PWM of some pins. Check if you can do a plain analogWrite in the **same** code.

###I want to fade more then 6 leds
By default the library can only fade 6 leds. Why 6? That's the number of PWM pins on a Uno (/Pro Mini/Nano). If you use a board with more then 6 hardware capable PWM pins you can increase this number by changing `scr\FadeLed.h`. Change

```C++
#define FADE_LED_MAX_LED  6 // <-- Increase to the number of objects you want.
```

###I want to fade a RGB led nicely.
Set the fade time of each color to the same time and to constant fade *time*. Now always set the brightness of all the three colors together (or at least all befor you call `FadeLed::update()`).

```C++
#include <FadeLed.h>

//the RGB led
FadeLed redLed(9);
FadeLed greenLed(10);
FadeLed blueLed(11);

void setup() {
  //set all colors to same time and to constant fade time
  //Each color change will take 5 seconds
  redLed.setTime(5000, true);
  greenLed.setTime(5000, true);
  blueLed.setTime(5000, true);
}

void loop() {
  //We always need to call this
  FadeLed::update();
  
  if(changeColor){
    //set each color to a new value
    redLed.set(newRedValue);
    greenLed.set(newGreenValue);
    blueLed.set(newBluevalue);
  }
}
```
