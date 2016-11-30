#include "Arduino.h"
#include "FadeLed.h"

unsigned int FadeLed::_interval = 50;
unsigned int FadeLed::_millisLast = 0;
byte FadeLed::_ledCount = 0;
FadeLed* FadeLed::_ledList[FADE_LED_MAX_LED];

FadeLed::FadeLed(byte pin) :_count(0),
                            _countMax(40),
                            _constTime(false),
                            _levelsAmount(FADE_LED_RESOLUTION){
  _pin = pin;
  
  //only add it if it fits
  if(_ledCount < FADE_LED_MAX_LED){
    _ledList[_ledCount++] = this;
  }
}

void FadeLed::begin(flvar_t val){
  //set to both so no fading happens
  _setVal = val;
  _curVal = val;
  analogWrite(this->_pin, _curVal);
}

void FadeLed::set(flvar_t val){
  
  /** edit 2016-11-17
   *  Fix so you can set it to a new value while 
   *  fading in constant speed. And impossible to set 
   *  a new value in constant time.
   */
  if(_setVal != val){
    /** edit 2016-11-30
     *  Fixed out of range possibility
     */
    //check for out of range
    if(val > _levelsAmount){
      //if bigger then allowed, use biggest value
      val = _levelsAmount;
    }    
    
    //if it's now fading we have to check how to change it
    if(!done()){
      //setting new val while fading in constant time not possible
      if(_constTime){
        return;
      }
      //if in constant speed the new val is in same direction and not passed yet
      else if(( (_startVal < _setVal) && (_curVal < val)) || //up
              ( (_startVal > _setVal) && (_curVal > val)) ){ //down
        //just set a new val
        _setVal = val;
        return;
      }
    }
    
    //if we make it here it's or finished fading
    //or constant speed in other direction
    //save and reset
    _setVal = val;
    _count = 1;
    
    //and start fading from current position
    _startVal = _curVal;
  }
  
  
   /* Old code 2016-11-17
>>>>>>> origin/master
  if(_setVal != val){
    //save and reset counter
    _setVal = val;
    _count = 1;

    //so we know where to fade from
    _startVal = _curVal;
  }*/
}

flvar_t FadeLed::get(){
  return _setVal;
}

flvar_t FadeLed::getCurrent(){
  return _curVal;
}

bool FadeLed::done(){
  return _curVal == _setVal;
}

void FadeLed::on(){
  this->set(_levelsAmount);
}

void FadeLed::off(){
  this->set(0);
}

void FadeLed::beginOn(){
  this->begin(_levelsAmount);
}

void FadeLed::setTime(unsigned long time, bool constTime){
  //Calculate how many times interval need to pass in a fade
  this->_countMax = time / _interval;
  this->_constTime = constTime;
}

bool FadeLed::rising(){
  return (_curVal < _setVal);
}

bool FadeLed::falling(){
  return (_curVal > _setVal);
}

void FadeLed::stop(){
  _setVal = _curVal;
}

void FadeLed::updateThis(){
  //need to fade up
  if(_curVal < _setVal){
    flvar_t newVal;
    
    //we always start at the current level saved in _startVal
    if(_constTime){
      //for constant fade time we add the difference over countMax steps
      newVal = _startVal + _count * (_setVal - _startVal) / _countMax;
    }
    else{
      //for constant fade speed we add the full resolution over countMax steps
      newVal = _startVal + _count * _levelsAmount / _countMax;
    }
    
    //check if new
    if(newVal != _curVal){
      //check for overflow
      if(newVal < _curVal){
        _curVal = _levelsAmount;
      }
      //Check for overshoot
      else if(newVal > _setVal){
        _curVal = _setVal;
      }
      //Only if the new value is good we use that
      else{
        _curVal = newVal;
      }
          
      analogWrite(this->_pin, _curVal);
    }
    _count++;
  }
  //need to fade down
  else if(_curVal > _setVal){
    flvar_t newVal;
    
    //we always start at the current level saved in _startVal
    if(_constTime){
      //for constant fade time we subtract the difference over countMax steps
      newVal = _startVal - _count * (_startVal - _setVal) / _countMax;
    }
    else{
      //for constant fade speed we subtract the full resolution over countMax steps
      newVal = _startVal - _count * _levelsAmount / _countMax;
    }
    
    //check if new
    if(newVal != _curVal){
      //check for overflow
      if(newVal > _curVal){
        _curVal = 0;
      }
      //Check for overshoot
      else if(newVal < _setVal){
        _curVal = _setVal;
      }
      //Only if the new value is good we use that
      else{
        _curVal = newVal;
      }
          
      analogWrite(this->_pin, _curVal);
    }
    _count++;
  }
  
}

void FadeLed::setInterval(unsigned int interval){
  _interval = interval;
}

void FadeLed::update(){
  unsigned int millisNow = millis();
  
  if(!_ledCount){
    return;
  }
  
  if(millisNow - _millisLast > _interval){
    //_millisLast = millisNow;
    //more accurate:
    _millisLast += _interval;
        
    //update every object
    for(byte i = 0; i < _ledCount; i++){
      _ledList[i]->updateThis();
    }
  }
}