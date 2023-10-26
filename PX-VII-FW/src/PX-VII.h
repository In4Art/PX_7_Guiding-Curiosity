/*
 * PX-VII Prosthetic X VII
 *
 * PX-VII is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * PX-VII is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with DotStar. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

 #ifndef PXVII_H_
#define PXVII_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_DotStarMatrix.h>

#include <SimpleGen.h>

#define WHITE 0xFFFF 
#define BLACK 0x0000
#define WHITE24 0xFFFFFF
#define BLACK24 0x000000

#define NUM_LEDS_PANEL 64
#define NUM_PANELS 1

#define BR_LOW 80
#define BR_HIGH 192


class PXVII : public Adafruit_DotStarMatrix{
    public:
        PXVII(int w, int h,  uint8_t matrixType, uint8_t ledType);
        void init(void);
        void update(void); //gather new data and process it
        void run(void); //respond to new data with output

        void setPanelBrightness(int8_t panel, uint8_t br);//set brightness br of panel

        void drawPixel24b(int16_t x, int16_t y, uint32_t color);
        static uint8_t sysState; //state can be 0 (calm) or 1 (active) 
        void setSysState(uint8_t st);

    private:

        uint32_t _updateDT = 100; //update data @10Hz
        uint32_t _updateT = 0; //last time update was executed

        uint32_t _runDt = 20; //run @ 50Hz
        uint32_t _runT = 0; //last time run was executed

        float _ramp = 0.f;
        float _rampSpeed = 8.f; // rampspeed in seconds
        float _rampIncr =  (1.f / _runDt) / _rampSpeed; //frames / second * seconds
        uint16_t _maxBright = 50;

        uint8_t _brightness[NUM_PANELS];
        uint8_t _p0brightness = 255;

        uint16_t _randRate = _runDt * 5;
        uint32_t _randT = 0;

        TriGen px0;

        TriGen px[NUM_LEDS_PANEL * NUM_PANELS];
       

};

#endif