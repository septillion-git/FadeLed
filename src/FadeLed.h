/**
 *  @file FadeLed.h
 *  @brief A simple Arduino library to fade leds on the hardware PWM.
 *  
 *  This library takes all the heavy lifting of fading a led. 
 */

#ifndef _FADE_LED_H
#define _FADE_LED_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/pgmspace.h>

/**
 *  @brief Sets the number of bits used for the PWM.
 *  
 *  Set this to a higher value for devices with higher PWM resolution. Will set the upper limit and variable type correspondingly.
 *  
 *  **Default** 8-bit: 0-255 for Uno/Nano/Pro Mini etc
 *  
 *  @warning Can't simply increase the number to have more PWM levels. It's limited to the hardware.
 */
#define FADE_LED_PWM_BITS 8


/**
 *  @brief Sets the variable type used for the brightness.
 *  
 *  Is done automatically according to #FADE_LED_PWM_BITS. A byte (uint8_t) is used for 8-bit or less. A unsigned int (uint16_t) is used for anything above. So limited to 16-bit PWM.
 *  
 *  @see FADE_LED_PWM_BITS
 */
#if FADE_LED_PWM_BITS <= 8
typedef byte flvar_t;
#else
typedef unsigned int flvar_t;
#endif


/**
 *  @brief Maximum number of FadeLed objects
 *  
 *  **Default** = 6, the number of hardware PWM pins on a Uno/Pro Mini/Nano
 */
#ifndef FADE_LED_MAX_LED
#define FADE_LED_MAX_LED  6
#endif

/**
 *  @brief The maximum brightness step of the PWM
 *  
 *  Automatically set depending on #FADE_LED_PWM_BITS. 255 for 8-bit.
 *  
 *  @warning Can't simply be increased for more then 8-bit because byte type is used to pass brightness values!
 *  
 *  @see FADE_LED_PWM_BITS
 */
#ifndef FADE_LED_RESOLUTION
#define FADE_LED_RESOLUTION ((1 <<FADE_LED_PWM_BITS) -1)
#endif

#include "FadeLedGamma.h"

/**
 *  @brief Main class of the FadeLed-library
 *  
 *  The FadeLed class includes all the functions to easily fade a led including a static function to update all constructed FadeLed objects.
 *  
 *  @see set(), update()
 */
class FadeLed{
  public:
    /**
     *  @brief Constructor of a FadeLed object
     *  
     *  @details Makes a object to fade a specific pin. You can make a object for each PWM pin you want to fade. Updating all objects is done by simply calling update()
     *  
     *  When created the default brightness is **0**. You can start at a different brightness by calling begin().
     *  
     *  @note Make all objects global (or static if you really must) because a destruct will lead to errors!
     *  
     *  @warning Don't make two objects for the same pin, they will conflict!
     *  
     *  @param [in] pin The PWM pin to fade with this object
     *  
     *  @see update(), set(), on(), off()
     */
    FadeLed(byte pin);
    
    /**
     *  @brief Set a direct begin value to start at without fade
     *  
     *  @details If you want to directly start the led at a certain brightness you can set it with this. This will directly set the brightness without fading.
     *  
     *  @note You can also use this to set a brightness without fading
     *  
     *  @see beginOn()
     *  
     *  @param [in] val The brightness to start at.
     */
    void begin(flvar_t val);
    
    /**
     *  @brief Set the brightness to fade to
     *  
     *  @details Set the brightness to which the led should fade to. 
     *  
     *  In **constant fade speed** if the new value is in the same fading direction as were started and the value is not yet passed the fade just continues to the new value
     *  
     *  In **constant fade time** a new value is **ignored** if the led is still fading
     *  
     *  Otherwise the fade is just reset and the led will start fading to the new brightness.
     *  
     *  @note To make all the fading work you need to call FadeLed::update() **often** in the loop()!
     *  
     *  @see done(), setTime()
     *  
     *  @param [in] val The brightness to fade to.
     */
    void set(flvar_t val);
    
    /**
     *  @brief Returns the last **set** brightness
     *  
     *  @details Returns the last **set** brightness via set() (or begin())
     *  
     *  @see set(), begin()
     *  
     *  @return The last set brightness
     */
    flvar_t get();
    
    /**
     *  @brief Returns the current brightness
     *  
     *  @details Returns the current brightness of the output
     *  
     *  @note It does not return the last set value. For that, use get()
     *  
     *  @see get()
     *  
     *  @return Current brightness of the led.
     */
    flvar_t getCurrent();
    
    /**
     *  @brief Returns if the led is done fading
     *  
     *  @details **true** if the led is done fading and reached the set() value.
     *  
     *  **false** if it's still fading
     *  
     *  @return If the led is fading or not
     */
    bool done();
    
    /**
     *  @brief Fade to max brightness
     *  
     *  @details Sets the led to fade to max brightness. Same as calling `set(255)`
     *  
     *  @see set()
     *  
     */
    void on();
    
    /**
     *  @brief Fade to off
     *  
     *  @details Sets the led to fade to off. Same as calling `set(0)`
     *  
     *  @see set()
     *  
     */
    void off();
    
    /**
     *  @brief Sets the start brightness directly to full.
     *  
     *  @details Short for calling `begin(255)`
     *  
     *  @see begin()
     */
    void beginOn();
    
    /**
     *  @brief Set the time a (full) fade will take
     *  
     *  @details This will set how much time a fade will take.
     *  
     *  The real fade time will be a **whole multiple of the set interval** (rounded down). For example, if interval is set to 50ms and you specify a fade time of 1025ms the fading time is actually set to 1000ms.
     *  
     *  **Constant fade speed** (default)\n
     *  In constant fade speed this is the time a fade from off to full brightness (or vice versa) will take. A fade of less will go with the same speed but will take less time
     *  
     *  **Constant fade time**\n
     *  In constant fade time this is the time each fade will take. No matter how much change in brightness. Each call to set() will reset the fading time and it will fade from it's current brightness to the new brightness in this time. Useful for example if you want to fade a RGB led from one color to another in a set time. No matter the level of the individual RGB colors. 
     *  
     *  @note If you want to change the update interval (setInterval()) do that before calling setTime(). The fade time is calculated using the interval.
     *  
     *  @see setInterval()
     *  
     *  @param [in] time      The time (ms) a fade will take
     *  @param [in] constTime **[optional]** true to use constant fade time. **Default** constant fading speed
     */
    void setTime(unsigned long time, bool constTime = false);
    
    /**
     *  @brief Returns if the led is still fading up
     *  
     *  @details **true** if fading up. 
     *  
     *  **false** otherwise
     *  
     *  @return if the led is fading up
     */
    bool rising();
    
        /**
     *  @brief Returns if the led is still fading down
     *  
     *  @details **true** if fading down. 
     *  
     *  **false** otherwise
     *  
     *  @return if the led is fading down
     */
    bool falling();
    
    /**
     *  @brief Stops the current fading
     *  
     *  @details Makes the current brightness the set brightness
     *  
     *  Useful if you fade to find the desired brightness
     */
    void stop();
    
    
    /**
     *  @brief Updates all FadeLed objects
     *  
     *  @details This is the core function of FadeLed. Calling this function will check each object of FadeLed to see if the brightness needs changing (fade). 
     *  
     *  It's a static function, you only need to call is once for all objects of FadeLed. You can call it using the class name like:
     *  
     *  ```C++    
     *  loop(){
     *    FadeLed::update();
     *  }
     *  ```
     *  
     *  @note Call this function **often** in order not to skip steps. Make the code non-blocking aka **don't** use delay() anywhere! See [Blink Without Delay()](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay)
     *  
     */
    static void update();
    
    /**
     *  @brief Sets the interval at which to update the fading
     *  
     *  @details Only every interval when calling update() it's checked to see if the brightness of the leds needs to change (fade) to leave time for other stuff. 
     *  
     *  **default:** 50ms
     *  
     *  @warning Call this before setting a fading time (via setTime()). Changing the interval will change the fading time or **each** FadeLed object.
     *  
     *  @see setTime()
     * 
     *  @param [in] interval Interval in ms
     */
    static void setInterval(unsigned int interval);
    
    flvar_t getGamma(flvar_t in);
    
    
  protected:
    byte _pin; //!< PWM pin to control
    flvar_t _setVal; //!< The brightness to which last set to fade to
    flvar_t _startVal; //!< The brightness at which the new fade needs to start
    flvar_t _curVal; //!< Current brightness
    bool _constTime; //!< Constant time fade or just constant speed fade
    unsigned long _countMax; //!< The number of #_interval's a fade should take
    unsigned long _count; //!< The number of #_interval's passed
    const flvar_t* _gammaLookup;
    flvar_t _gammaSteps;
    
    
    
    /**
     *  @brief Updates fading of this object only
     *  
     *  @details Can't be called directly (it's protected). Instead call update() to update all FadeLed objects.
     *  
     *  @see update()
     *  
     */
    void updateThis();
    
    
    
    static FadeLed* _ledList[FADE_LED_MAX_LED]; //!< array of pointers to all FadeLed objects
    static byte _ledCount; //!< Next number of FadeLed object
    static unsigned int _interval; //!< Interval (in ms) between updates
    static unsigned int _millisLast; //!< Last time all FadeLed objects where updated
};

inline flvar_t FadeLed::getGamma(flvar_t in){
  return pgm_read_byte_near(_gammaLookup + in);
}


#endif