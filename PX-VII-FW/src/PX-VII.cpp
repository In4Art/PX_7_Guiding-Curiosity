

#include "PX-VII.h"

void maxRandomizer(TriGen *genPtr);
 uint8_t PXVII::sysState = 0;

PXVII::PXVII(int w, int h,  uint8_t matrixType, uint8_t ledType) : 
  Adafruit_DotStarMatrix( w, h, matrixType, ledType){}

void PXVII::init(void){

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  begin();
  fillScreen(BLACK);
  setBrightness(255);
  show();

  for(int i = 0; i < NUM_PANELS; i++){
    _brightness[i] = 255;//BR_LOW;
  }
  for(int i = 0; i < NUM_LEDS_PANEL * NUM_PANELS; i++){
    px[i].setSamprate(1000.f / _runDt);
    px[i].setMax(0.25);
    px[i].setFreq(0.25);
    px[i].setPeriodCB(maxRandomizer);
  }
  PXVII::sysState = 0;
 
}

void PXVII::update(void){

  _updateT = millis();
}

void PXVII::run(void){
  uint32_t currT = millis();

  if(currT - _runT > _runDt){


  
  for(int p = 0; p < NUM_PANELS; p++){
    for (int i = 0 + (p * NUM_LEDS_PANEL); i <  ( (p + 1) * NUM_LEDS_PANEL); i++){

     float val = px[i].calc();
      uint8_t pxBrightness = (uint8_t)(val * _brightness[p]);
      uint8_t rBright = (uint8_t)( (val * 0.7 ) * _brightness[p]);
      
      setPixelColor(i, (pxBrightness << 16) | (rBright << 8)  | pxBrightness);
    }
  }


      show();

      _runT = currT;
  }

  
}

void PXVII::drawPixel24b(int16_t x, int16_t y, uint32_t color){
    setPassThruColor(color);
    drawPixel(x, y, 0);
}

void PXVII::setPanelBrightness(int8_t panel, uint8_t br){
  _brightness[panel] = br;
}

void PXVII::setSysState(uint8_t st){
  PXVII::sysState = st;
}

void maxRandomizer(TriGen *genPtr){

  //randomSeed(millis());
  if(PXVII::sysState == 0){
    float f = (((float)random(RAND_MAX))/(float)RAND_MAX) * 0.5;
    genPtr->setFreq(f);
    float b = (((float)random(RAND_MAX))/(float)RAND_MAX) * 0.25 * ((float)BR_LOW / 255.f);
    if(b < 0.05){
      genPtr->setMax(0.f);
    }else{
      genPtr->setMax(b);
    }
  }else if(PXVII::sysState == 1){
    float f = ((((float)random(RAND_MAX))/(float)RAND_MAX) * 3.5) + 0.5;
    genPtr->setFreq(f);
    float b = (((((float)random(RAND_MAX))/(float)RAND_MAX) * 0.75) + 0.25) * ((float)BR_HIGH /  255.f);
    genPtr->setMax(b);
  }
}


