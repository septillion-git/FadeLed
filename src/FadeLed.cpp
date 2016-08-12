#include "Arduino.h"
#include "FadeLed.h"

unsigned int FadeLed::_interval = 50;
unsigned int FadeLed::_millisLast = 0;
byte FadeLed::_ledCount = 0;
FadeLed* FadeLed::_ledList[FADE_LED_MAX_LED];

FadeLed::FadeLed(byte pin) :_count(0),
                            _countMax(40),
                            _constTime(false){
  _pin = pin;
  
  //only add it if it fits
  if(_ledCount < FADE_LED_MAX_LED){
    _ledList[_ledCount++] = this;
  }
}

void FadeLed::begin(byte val){
  _setVal = val;
  _curVal = val;
  analogWrite(this->_pin, _curVal);
}

void FadeLed::set(byte val){
  _setVal = val;
  _count = 1;
  _startVal = _curVal;
}

byte FadeLed::get(){
  return _setVal;
}

bool FadeLed::done(){
  return _curVal == _setVal;
}

void FadeLed::on(){
  this->set(FADE_LED_RESOLUTION);
}

void FadeLed::off(){
  this->set(0);
}

void FadeLed::beginOn(){
  this->begin(FADE_LED_RESOLUTION);
}

void FadeLed::setTime(unsigned long time, bool constTime){
  this->_countMax = time / _interval;
  this->_constTime = constTime;
}

bool FadeLed::rising(){
  return (_curVal < _setVal);
}

bool FadeLed::falling(){
  return (_curVal > _setVal);
}

void FadeLed::updateThis(){
  //need to fade up
  if(_curVal < _setVal){
    byte newVal;
    if(_constTime){
      newVal = _startVal + _count * (_setVal - _startVal) / _countMax;
    }
    else{
      newVal = _startVal + _count * FADE_LED_RESOLUTION / _countMax;
    }
    
    //check if new
    if(newVal != _curVal){
      //check for overflow
      if(newVal < _curVal){
        _curVal = FADE_LED_RESOLUTION;
      }
      //Check for overshoot
      else if(newVal > _setVal){
        _curVal = _setVal;
      }
      else{
        _curVal = newVal;
      }
          
      analogWrite(this->_pin, _curVal);
    }
    _count++;
  }
  //need to fade down
  else if(_curVal > _setVal){
    byte newVal;
    if(_constTime){
      newVal = _startVal - _count * (_startVal - _setVal) / _countMax;
    }
    else{
      newVal = _startVal - _count * FADE_LED_RESOLUTION / _countMax;
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
    //while to compensate if not called in time <<<------
    while(millisNow - _millisLast > _interval){
      _millisLast += _interval;
    }
    
    
    for(byte i = 0; i < _ledCount; i++){
      _ledList[i]->updateThis();
    }
  }
}