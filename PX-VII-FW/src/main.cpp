
#include <Arduino.h>


#include "PX-VII.h"


#include <ESP8266WiFi.h>

#include <ModbusIP_ESP8266.h>

#include "WifiControl.h"
#include "ModeControl.h"

#include "creds.h"


#define PX_NUM 7
#define PX_REG 100 + (PX_NUM * 10) //base operational modbus register

#define PX_STATE_REG 200 + (PX_NUM * 10) // base status modbus register

enum {
  PX_ERR = -1,
  PX_OK
};

char ssid[] = SSID ;        // your network SSID (name)
char pass[] = PW;                    // your network password
WifiControl pxWifi(ssid, pass, PX_NUM);
ModbusIP pxModbus;

int8_t px7State = 0;

PXVII px7 = PXVII(8 * NUM_PANELS, 8,
  DS_MATRIX_TOP     + DS_MATRIX_RIGHT +
  DS_MATRIX_COLUMNS + DS_MATRIX_PROGRESSIVE,
  DOTSTAR_BRG);

#define DEMO_SW_PIN D2
void demoCallback(uint32_t dTime, px_mode_t mode);
ModeControl pxMC(DEMO_SW_PIN, &demoCallback, 10000, &pxWifi);

void wifiEmptyFunc();




void setup() {

   px7.init();



  Serial.begin(57600);
  delay(2000);


   //time out used for waiting for a connection to occur
  //useful to change during testing to speed things up when dev-ing with no C&C
  pxWifi.setTimeOut(30000);


  if(digitalRead(DEMO_SW_PIN) == HIGH){
    Serial.println("Connecting to C&C...");
    int8_t res = pxWifi.init();
    if(res == -1){
      Serial.println("No C&C found, starting up in demo mode!");
    }
  }
  
  //Serial.println("connected");
  pxModbus.server(502);
  pxModbus.addHreg(PX_REG, 0);
  pxModbus.addIreg(PX_REG, 0);;
  pxModbus.addHreg(PX_STATE_REG, PX_OK);

  pxMC.init();//initialize modeControl
  Serial.println("setup finished.");
 

}

void setState(int8_t state)
{
  px7State = state;
  if(px7State > 0){
    px7.setSysState(1);
  }else{
    px7.setSysState(0);
  }
}


uint32_t runCheck = 0;

void loop() {


  px7.update();

  px7.run();


  pxModbus.task();

  
  pxMC.run();
  
  pxWifi.run(); //automatically checks wifi status and try to reconnect after timeout 


  if(pxModbus.Hreg(PX_REG) != pxModbus.Ireg(PX_REG)){
    pxModbus.Ireg(PX_REG, pxModbus.Hreg(PX_REG));
  }

  if(px7State != pxModbus.Ireg(PX_REG)){
    setState((int8_t)pxModbus.Ireg(PX_REG));
  }

  
 
}

void demoCallback(uint32_t dTime, px_mode_t mode){
  // the demoCallback is also called when switching from demo mode back to C&C mode
  // and on a loss of wifi connection
  // therefor the mode is also passed to the callback
  // like this it is possile to set the prosthetic to a specific state after leavin demo mode
  if (mode == PX_DEMO_MODE)
  {
    if (pxModbus.Ireg(PX_REG) >= 1)
    {
      pxModbus.Hreg(PX_REG, 0);
    }
    else
    {
      pxModbus.Hreg(PX_REG, 1);
    }
  }
  else
  {
    pxModbus.Hreg(PX_REG, 0);
  }
}

void wifiEmptyFunc()
{

}