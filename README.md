# FadeLed
Makes fading LEDs on an Arduino easy.
By Timo Engelgeer (Septillion)

***New in version v1.4.0:* Gamma correction.**
As of version v1.4.0 the library fades with gamma correction. This is now the default behavior. This also sets the default range to 0 to 100 (instead of the old 0 to 255 for 8-bit). So if you upgrade from an older version change the range in your sketch or disable gamma correction (`noGammaTable()`).

## What does it do?
Fading is easy right? But it can quickly become quite some code to do so. FadeLed does all the heavy lifting for you. It can fade every LED on a *hardware* PWM pin (capable of `analogWrite()`) with gamma correction. Just make a FadeLed object for it, set the fade time and just set the brightness to fade to. You can let it fade to and from each brightness you like!

### Fade modes
FadeLed can fade in two modes, constant fade speed (default) and constant fade time. The mode can be selected for each LED.

#### Constant fade speed
This is the default mode. A LED will fade with a constant speed. The time you set is the time of a full fade from off to full brightness or vice versa. If the fade is not full scale, for example to/from half brightness, it will take less time. In the case of half brightness it will take half the time.

#### Constant fade time
Each fade will now take the same amount of time. No matter if you fade the full scale or just by 10 steps, it will take the same time. 

### Gamma correction
The human eye doesn't respond linearly to light. It's way more sensitive for small changes in dim light than it is to small changes in bright light. To compensate for that FadeLed uses gamma correction tables. This gives a very natural feel to the dimming and fading. Even if you're not interested in fading you can use FadeLed to drive LEDs with gamma correction.

If desired, the gamma correction can be disabled per LED. It's also possible to use a different gamma table than default (with a gamma of 2,3), even per LED!

## Download and install
### Library manager
FadeLed is available via Arduino IDE Library Manager.

1.  Open the Arduino IDE (1.5 or above).
2.  In the tool-bar click Sketch -> Include Library -> Manage Libraries...
3.  Type in the search bar "FadeLed".
4.  The latest version of FadeLed should show.
5.  Click on it and click Install.
6.  Done!

### GitHub
Latest release: **[v1.6.0](https://github.com/septillion-git/FadeLed/archive/v1.6.0.zip)**

1. Download the latest release.
2. Extract it to the `libraries` folder **inside** your Sketchbook. Default is `[user]\Arduino\libraries`. 
3. Rename the folder to `FadeLed` (remove version number).
4. Restart the Arduino IDE if you had it open.
5. Done!

### Update
You can update to the latest version of the library in the Library Manager as well. 

1.  Open the Arduino IDE (1.5 or above).
2.  In the tool-bar click Sketch -> Include Library -> Manage Libraries...
3.  Type in the search bar "FadeLed".
4.  The FadeLed library should show.
5.  Click on it and click Update.
6.  Done!

Alternatively you can download it from GitHub and simply unpack it over the current version (or remove the old version first).

## Usage
### FadeLed-object
Using FadeLed is simple. Just make a FadeLed object for each LED you want to fade like

```C++
FadeLed statusLed(5); //Fading status LED on pin 5
FadeLed anotherLed(6); //Other fading LED on pin 6
```

It's also possible to make an array of multiple FadeLed-objects which can make it easy to loop over each.

```C++
FadeLed myLeds[] = {5, 6}; //creates two FadeLed objects: leds[0] and leds[1]
```

### void FadeLed::update()
In order to update the fading of all FadeLed objects you just have to call `FadeLed::update()` frequently. So it's best to have a non-blocking loop and call it like
```C++
void loop(){
  FadeLed::update(); //updates all FadeLed objects
  
  /*
  Do all the other things you want to do
  */
}
```

### void .setTime(unsigned long time)
Calling this function will set the time a fade needs to take. By default it will set the time a full fade should take (constant fade speed). But by entering `true` as second parameter you can change that to the time each fade should take (constant fade time). You can change the fade time anytime you like! The time is set in **milliseconds**

```C++
statusLed.setTime(5000); //makes the statusLed fade with constant fade speed. Full fade will take 5 seconds
anotherLed.setTime(1000, true); //makes this LED fade with constant fade time. Each fade now takes 1 second
```

### void .set(byte value)
The most common function of the library. Simply sets the brightness to fade to.

```C++
statusLed.set(127); //fades the statusLed to half brightness
```

There are also the shortcuts `.on()` and `.off()` to simply fade to full on or full off respectively.

## More methods
Other useful methods of the library include `.on()`, `.off()`, `.done()`, `.get()`, `.rising()`, `.falling()` and `FadeLed::setInterval()`. For documentation of all the methods, see the full documentation.

## Full documentation
Full documentation of all the methods of this library can be found inside the library located in `FadeLed\doc`. Just open `FadeLed\doc\index.html` to see all methods of FadeLed. 

You can also view the documentation via [GitHub HTML Preview](https://htmlpreview.github.io/?https://github.com/septillion-git/FadeLed/blob/v1.6.0/doc/index.html).

This documentation is powered by [Doxygen](http://www.doxygen.org/) and thus fully extracted from the source files. This README.md is also used as Main Page.

## FAQ

### My LED doesn't fade nice/all the time!
Check to see if `FadeLed::update()` is called regularly. So you should **not** use blocking code! The `loop()` should run freely. The biggest example of blocking code is the `delay()` function. (See [Blink without delay](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) how to fix that.) But also other functions can block like `Serial.readBytesUntil()`, `Serial.parseInt()` or other functions that just wait until something happens.

### Nothing happens!
Are you calling `FadeLed::update()` frequently? Have you used a PWM (capable of `analogWrite()`) pin? 

If you do, are you using another library (or code) that uses a timer? For example `Servo`. This will block the PWM of some pins. Check if you can do a plain analogWrite in the **same** code.

### I want to fade more than 6 LEDs
By default the library can only fade 6 LEDs. Why 6? That's the number of PWM pins on an Uno (/Pro Mini/Nano). If you use a board with more than 6 hardware capable PWM pins you can increase this number by changing `src\FadeLed.h`. Change

```C++
#define FADE_LED_MAX_LED  6 // <-- Increase to the number of objects you want.
```

### I want to fade a RGB LED nicely.
Set the fade time of each color to the same time and to constant fade *time*. Now always set the brightness of all the three colors together (or at least all before you call `FadeLed::update()`).

```C++
#include <FadeLed.h>

//the RGB LED
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

### I have a device with more than 8-bit PWM
Simply change the macro `FADE_LED_PWM_BITS` in `FadeLed.h` to the number of bits your device has. 

For example for a with 10-bit PWM:
```C++
//Change this to match the number of PWM bit on other devices
#define FADE_LED_PWM_BITS 10
```

Selection is only automated for ESP8266. Would like to automate this in the future for more devices.

### Nothing changes when I call FadeLed.set() in constant fade time
Calling FadeLed.set() is ignored while the LED is still fading in **constant fade time** (not in constant fade speed). Wait until it's done (check FadeLed.done() ) or call FadeLed.stop() to stop at the current brightness after which you can set a new brightness to fade to.

### Error compiling for Digispark
FadeLed **does** work on ATtiny85. But when you try to compile it for a Digispark you end up with
```
FadeLedGamma.h:10:39: error: 'nullptr' was not declared in this scope
```
This is unfortunally because the [Digispark board](https://github.com/digistump/DigistumpArduino) definition isn't updated in over two year. :pensive: C++11 isn't enabled for Digispark. You can enable it by editing the `platform.txt` of the Digispark. This file is located in
```
C:\Users\[User]\AppData\Local\Arduino15\packages\digistump\hardware\avr\1.6.7
```
Find the line starting with `compiler.cpp.flags` and add `-std=gnu++11`. Total line will look like
```
compiler.cpp.flags=-c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -std=gnu++11
```
Restart the Arduino IDE and it should compile for a Digispark as well!

### What license does apply to FadeLed?
FadeLed is licenced under GPLv3. See `LICENSE`-file in the root of this project for more info.
