/**
 *  @file
 *  @Author Septillion (https://github.com/sseptillion)
 *  @date 2018-04-27
 *  @brief Example how to use FadeLed library with custom gamma table.
 *  
 *  @details This is an example how to use FadeLed library with custom gamma table.
 *  In this example it even uses a sinusoidal table. You're completely free how
 *  the graph should look like, simply define a value for each step.
 *  
 *  The sine table is made with the included Python script 'makeSineTable.py' in the 
 *  folder of this example. Running that will give 91 values (0 to 90 degree) to 
 *  form a sine.
 *  
 *  The pastern that is followed by pins 9, 10 and 11 is shown in 'sines.png'
 *  @image html sines.png
 *  
 */

#include <FadeLed.h>

//91 step sine table 0 = 0 degree, 90 = 90 degree
const flvar_t SineTable[91] PROGMEM = {
  0,   4,   9,  13,  18,  22,  27,  31,  35,  40,
 44,  49,  53,  57,  62,  66,  70,  75,  79,  83,
 87,  91,  96, 100, 104, 108, 112, 116, 120, 124,
127, 131, 135, 139, 143, 146, 150, 153, 157, 160,
164, 167, 171, 174, 177, 180, 183, 186, 190, 192,
195, 198, 201, 204, 206, 209, 211, 214, 216, 219,
221, 223, 225, 227, 229, 231, 233, 235, 236, 238,
240, 241, 243, 244, 245, 246, 247, 248, 249, 250,
251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
255};

//Make FadeLed object with a sine lookup table of 91 (0 to 90 including)
FadeLed sines[3] = {{9, SineTable, 90}, {10, SineTable, 90}, {11, SineTable, 90}};
const byte NrSines = sizeof(sines)/sizeof(sines[0]); //Number of sines

void setup(){
  //loop all sines to set them to the same (constant speed) fading time
  for(byte i = 0; i < NrSines; i++){
    sines[i].setTime(5000); //halve period, 0% to 90% or 90% to 0%
  }
  
  //Turn on the first to get started
  sines[0].on();
}

void loop(){
  FadeLed::update();
  
  //If all sines are off, start over by turning the first sine on.
  if(allOff()){
    delay(1000);
    sines[0].on();
  }
 
  for(byte i = 0; i < NrSines; i++){
    //if sin is 100% (90 degree), fade back down
    if(sines[i].done() && sines[i].get()){
      sines[i].off();
    }
    
    //for all but first
    if(i > 0){
      //if previous sine is >45 degree and this sine is off and not fading, start fading
      if( (sines[i - 1].getCurrent() >= 45) 
          && sines[i].done() 
          && (sines[i].get() == 0) )
      {
        sines[i].on();
      }
    }
  }
}

//returns true is all sines are turned off (at step 0)
bool allOff(){
  bool rtn = true;
  
  //check all sines, if a sine is not currently off (step 0)
  // or set to off (step 0), it set rtn to false.
  for(byte i = 0; i < NrSines; i++){
    rtn &= !sines[i].getCurrent();
    rtn &= !sines[i].get();
  }
  
  return rtn;
}