/**
 *  @file FadeLed.h
 *  @brief A simple Arduino library to fade LEDs on the hardware PWM.
 *  
 *  This library takes all the heavy lifting of fading a LED. 
 */

#ifndef _FADE_LED_H
#define _FADE_LED_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if defined ( ESP8266 )
  #include <pgmspace.h>
#else
  #include <avr/pgmspace.h>
#endif

/**
 *  @brief Sets the number of bits used for the PWM.
 *  
 *  Set this to a higher value for devices with higher PWM resolution. Will set the upper limit and variable type correspondingly.
 *  
 *  **Default** 8-bit: 0-255 for Uno/Nano/Pro Mini etc
 *  
 *  @warning Can't simply increase the number to have more PWM levels. It's limited to the hardware.
 */
 #if defined ( ESP8266 )
  //ESP8266 has 10-bit PWM
  #define FADE_LED_PWM_BITS 10
#else
  //Change this to match the number of PWM bit on other devices
  #define FADE_LED_PWM_BITS 8
#endif



/**
 *  @brief Sets the variable type used for the brightness.
 *  
 *  Is done automatically according to #FADE_LED_PWM_BITS. A byte (uint8_t) is used for 8-bit or less. An unsigned int (uint16_t) is used for anything above. So limited to 16-bit PWM.
 *  
 *  @see FADE_LED_PWM_BITS
 */
#if FADE_LED_PWM_BITS <= 8
typedef uint8_t flvar_t;
#else
typedef uint16_t flvar_t;
#endif


/**
 *  @brief Maximum number of FadeLed objects
 *  
 *  **Default** = 6, the number of hardware PWM pins on an Uno/Pro Mini/Nano
 */
#ifndef FADE_LED_MAX_LED
#define FADE_LED_MAX_LED  6
#endif

/**
 *  @brief The maximum brightness step of the PWM
 *  
 *  Automatically set depending on #FADE_LED_PWM_BITS. 255 for 8-bit.
 *  
 *  @warning Can't simply be increased for resolution! It depends on the resolution of the hardware used. Set it using #FADE_LED_PWM_BITS.
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
 *  The FadeLed class includes all the functions to easily fade a LED including a static function to update all constructed FadeLed objects.
 *  
 *  @see set(), update()
 */
class FadeLed{
  public:
    /**
     *  @brief Simple constructor of a FadeLed object with gamma correction.
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
     *  @see FadeLed(byte, const flvar_t*, flvar_t), update(), set(), on(), off()
     */
    FadeLed(byte pin);
    
    /**
     *  @brief Constructor of a FadeLed object
     *  
     *  @details Makes a object to fade a specific pin. You can make a object for each PWM pin you want to fade. Updating all objects is done by simply calling update()
     *  
     *  When created the default brightness is **0**. You can start at a different brightness by calling begin().
     *  
     *  With this constructor you can supply your own gamma table as gammaLookup. This must be an array of type flvar_t and should be placed in **PROGMEM**. Specify the largest steps in that table as biggestStep. For an example see the 'SineFade.ino' example in the examples folder.
     *  
     *  ```C++
     *  //put gamma table in PROGMEM
     *  const flvar_t myGammaTable[20] PROGMEM = {....};
     *  
     *  FadeLed ledCustom = {Pin, myGammaTable, 19};
     *  ```
     *  
     *  @note Make all objects global (or static if you really must) because a destruct will lead to errors!
     *  
     *  @warning Don't make two objects for the same pin, they will conflict!
     *  
     *  @param [in] pin         The PWM pin to fade with this object
     *  @param [in] gammaLookup Gamma table of type flvar_t in PROGMEM
     *  @param [in] biggestStep The largest possible value of the gamma table (gammaLookup).
     *  
     *  @see setGammaTable(), FadeLed(byte), FadeLed(byte, bool), update(), set(), on(), off()
     */
    FadeLed(byte pin, const flvar_t* gammaLookup, flvar_t biggestStep);
    
    /**
     *  @brief Simple constructor of a FadeLed object
     *  
     *  @details Simalair to FadeLed(byte) but with the possibility to disable the use of a gamma table by specifying false as hasGammaTable.
     *  
     *  @param [in] pin           The PWM pin to fade with this object
     *  @param [in] hasGammaTable **false** to disable the use of a gamma table, **true** to use the default gamma table.
     *  
     *  @see FadeLed(byte)
     */
    FadeLed(byte pin, bool hasGammaTable);
    
    /**
     *  @brief Simple destructor of a FadeLed object
     *  
     *  @details Destroy your FadeLed object and removes it from the FadeLed::update() cycle.
     */
    ~FadeLed();

    /**
     *  @brief Set a direct begin value to start at without fade
     *  
     *  @details If you want to directly start the LED at a certain brightness you can set it with this. This will directly set the brightness without fading.
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
     *  @details Set the brightness to which the LED should fade to. 
     *  
     *  In **constant fade speed** if the new value is in the same fading direction as were started and the value is not yet passed the fade just continues to the new value
     *  
     *  In **constant fade time** a new value is **ignored** if the LED is still fading
     *  
     *  Otherwise the fade is just reset and the LED will start fading to the new brightness.
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
     *  @return Current brightness of the LED.
     */
    flvar_t getCurrent();
    
    /**
     *  @brief Returns if the LED is done fading
     *  
     *  @details **true** if the LED is done fading and reached the set() value.
     *  
     *  **false** if it's still fading
     *  
     *  @return If the LED is fading or not
     */
    bool done();
    
    /**
     *  @brief Fade to max brightness
     *  
     *  @details Sets the LED to fade to max brightness. Same as calling `set(255)`
     *  
     *  @see set()
     *  
     */
    void on();
    
    /**
     *  @brief Fade to off
     *  
     *  @details Sets the LED to fade to off. Same as calling `set(0)`
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
     *  In constant fade time this is the time each fade will take. No matter how much change in brightness. Each call to set() will reset the fading time and it will fade from it's current brightness to the new brightness in this time. Useful for example if you want to fade a RGB LED from one color to another in a set time. No matter the level of the individual RGB colors. 
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
     *  @brief Returns if the LED is still fading up
     *  
     *  @details **true** if fading up. 
     *  
     *  **false** otherwise
     *  
     *  @return if the LED is fading up
     */
    bool rising();
    
        /**
     *  @brief Returns if the LED is still fading down
     *  
     *  @details **true** if fading down. 
     *  
     *  **false** otherwise
     *  
     *  @return if the LED is fading down
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
     *  @brief Sets a gamma table to use
     *  
     *  @details Let this FadeLed object use a specific gamma table. This table **must** be put in PROGMEM to work. #flvar_t can be used as variable type to get a variable type that matches the (set) bit resolution of the PWM.
     *  
     *  ```C++
     *  //put gamma table in PROGMEM
     *  const flvar_t myGammaTable[20] PROGMEM = {....};
     *  
     *  //link it to an object
     *  //biggestStep is size - 1!
     *  led.setGammaTable(myGammaTable, 19)
     *  ```
     *  
     *  Pointing to a nullptr will result in **no** gamma correction. biggestStep will then limit the brightness. But easier to use noGammaTable().
     *  
     *  ```C++
     *  //No gamma table, 8-bit PWM
     *  led.setGammaTable(nullptr, 255);
     *  ```
     *  
     *  It will **stop** the current fading. It also **resets** it to start from 0 for the next fade. 
     *  
     *  If you want to fade from the current brightness with the new gamma table you have to find the starting value yourself and set it via begin().
     *  
     *  By default a 101 steps (0-100 aka percentage) table is used with a gamma of 2,3. To generate a table with a different gamma you can use the provided Python script ('FadeLed\extras\GammaTable.py'). (You need to install Python for it to work!) Call it like: `python GammaTable.py Gamma Steps PWMbits [VariableName]`. VariableName is optional. For example `python gamma.py 2.5 50 10` will result in a table with 50 steps (0 - 49) with gamma = 2,5 for a 10-bit PWM. This will be stored in gamma.h and can be copy pasted into your code.
     *  
     *  @note It stops and resets but does **not** change the PWM output. This only gets changed after a new call to set(), on(), off(), begin() or beginOn(). If no action is taken an abrupt jump will happen if not at zero brightness.
     *  
     *  @param [in] table The gamma table in PROGMEM
     *  @param [in] biggestStep The biggest step of that gamma table (aka size -1) If no parameter is used 100 is assumed to be the top value possible.
     */
    void setGammaTable(const flvar_t* table, flvar_t biggestStep = 100);
    
    /**
     *  @brief Use no gamma correction for full range
     *  
     *  @details Let this object use no gamma correction and just use the full PWM range. 
     *  
     *  It's short for `setGammaTable(nullptr, FADE_LED_RESOLUTION)`
     *  
     *  @see setGammaTable(), FADE_LED_RESOLUTION
     */
    void noGammaTable();
    
    /**
     *  @brief Get gamma corrected value
     *  
     *  @details Gives the gamma corrected output level. Checks for the biggest possible step.
     *  
     *  @see getGamma()
     *  
     *  @param [in] step The step to get the gamma corrected output level for. Limited to the biggest possible value
     *  @return The gamma corrected output level if a gamma table is used, otherwise it returns in.
     */
    flvar_t getGammaValue(flvar_t step);
    
    /**
     *  @brief Get the biggest brightness step
     *  
     *  @details Gives the biggest brightness step for the gamma table in use. If no table is in use, it returns the biggest output level.
     *  
     *  @return Biggest brightness step
     */
    flvar_t getBiggestStep();
    
    
    /**
     *  @brief Updates all FadeLed objects
     *  
     *  @details This is the core function of FadeLed. Calling this function will check each object of FadeLed to see if the brightness needs changing (fade). 
     *  
     *  It's a static function, you only need to call it once for all objects of FadeLed. You can call it using the class name like:
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
     *  @details Only every interval when calling update() it's checked to see if the brightness of the LEDs needs to change (fade) to leave time for other stuff. 
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
    
  protected:
    const byte _pin; //!< PWM pin to control
    flvar_t _setVal; //!< The brightness to which last set to fade to
    flvar_t _startVal; //!< The brightness at which the new fade needs to start
    flvar_t _curVal; //!< Current brightness
    bool _constTime; //!< Constant time fade or just constant speed fade
    unsigned long _countMax; //!< The number of #_interval's a fade should take
    unsigned long _count; //!< The number of #_interval's passed
    const flvar_t* _gammaLookup; //!< Pointer to the Gamma table in PROGMEM
    flvar_t _biggestStep; //!< The biggest input step possible

    
    
    /**
     *  @brief Updates fading of this object only
     *  
     *  @details Can't be called directly (it's protected). Instead call update() to update all FadeLed objects.
     *  
     *  @see update()
     *  
     */
    void updateThis();
    
    /**
     *  @Brief Gives the output level for a given gamma step
     *  
     *  @details Looks it up in the PROGMEM gamma table for this object if table is assigned. Deals with the variable size used.
     *  
     *  Can't be called directly (it's protected) but it's inline for speed. If you want to get a gamma value for a given step, use getGammaValue() instead.
     *  
     *  @return Corresponding level for the given step. Returns in if no table is in use.
     *  
     *  @note Does **not** check for out of range! That's up to the caller. For that, use getGammaValue().
     *  
     *  @see getGammaValue() 
     *  
     *  @param [in] step The step to get the gamma corrected output level for.
     *  @return The gamma corrected output level if a gamma table is used, otherwise it returns in.
     */
    flvar_t getGamma(flvar_t step);
    
    static FadeLed* _ledList[FADE_LED_MAX_LED]; //!< array of pointers to all FadeLed objects
    static byte _ledCount; //!< Next number of FadeLed object
    static unsigned int _interval; //!< Interval (in ms) between updates
    static unsigned int _millisLast; //!< Last time all FadeLed objects where updated
};

inline flvar_t FadeLed::getGamma(flvar_t step){
  if(_gammaLookup == nullptr){
    return step;
  }
  else{
    #if FADE_LED_PWM_BITS <= 8
      return pgm_read_byte_near(_gammaLookup + step);
    #else
      return pgm_read_word_near(_gammaLookup + step);
    #endif
  }
}

#endif