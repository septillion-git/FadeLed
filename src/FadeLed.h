#ifndef _FADE_LED_H
#define _FADE_LED_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FADE_LED_MAX_LED  6

#ifndef FADE_LED_RESOLUTION
#define FADE_LED_RESOLUTION 255
#endif

class FadeLed{
  public:
    FadeLed(byte pin);
    void begin(byte val);
    void set(byte val);
    byte get();
    bool done();
    void on();
    void off();
    void beginOn();
    void setTime(unsigned long time);
    bool rising();
    bool falling();
    
    static void update();
    static void setInterval(unsigned int interval);
  protected:
    byte _pin;
    byte _setVal;
    byte _startVal;
    byte _curVal;
    unsigned long _countMax;
    unsigned long _count;
    
    void updateThis();
    
    static FadeLed* _ledList[FADE_LED_MAX_LED];
    static byte _ledCount;
    static unsigned int _interval;
    static unsigned int _millisLast;
};


#endif