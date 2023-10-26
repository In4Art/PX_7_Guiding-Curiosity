#include <SimpleGen.h>

TriGen::TriGen(void){
    _ramp = 0;
    _incr = 0;
    _max = 1.f;
    _sRate = 0;

    _periodCB = nullptr;

}

TriGen::TriGen(float freq, float max, float samprate){

    _sRate = samprate;

    _max = max;
    if(_sRate > 0.f){
        _incr = freq / _sRate;
    }else{
        _incr = 0;
    }

}

void TriGen::setFreq(float freq){
    if(_sRate > 0.f){
        _incr = (freq / _sRate) * 2;
    }else{
        _incr = 0;
    }
}

void TriGen::setMax(float max){
    _max = max;
}
void TriGen::setSamprate(float samprate){
    _sRate = samprate;
}

void TriGen::setPeriodCB(void (*callBack)(TriGen *genPtr)){
    _periodCB = callBack;
}

float TriGen::calc(void){

    float out = 0.f;

    _ramp += _incr;

    if(_ramp >  2.0 ){
        _ramp = 0;
        if(_periodCB != nullptr){
            _periodCB(this);
        }
      }

    if(_ramp < 1.0){
        out = _ramp * _max;
      }else{
          out = (2.f - _ramp) * _max;
        
      }
      return out;
    
}