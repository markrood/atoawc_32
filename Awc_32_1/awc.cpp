#include <atoawc_util.h>

#include <Arduino.h>
#include"AtoAwc_Util.h"
#include "awc.h"


Awc::Awc(AtoAwcUtil *atoAwcUtil)
{
  _atoAwcUtil = atoAwcUtil;  
}


//////////////////////////////////////////////////////////////
//                                                          //
//   doWaterChange                                          //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   description:  runs the automatic water change (awc)    //
//                  1.)empties water to lower level         //
//                  2.)fills water back up to normal level  //
//                  calculates and stores how long both     //
//                  emty and fill took the first time and   //
//                  uses padded time to determin if either  //
//                  fill or empty took too long.            //
//                                                          //
//////////////////////////////////////////////////////////////
void Awc::doWaterChange(){
  if( executingAwc == true){
    if(waterEmptied == false && waterFilled == true){
      Serial.println("Executing AWC");
      //_atoAwcUtil->webSerialWrite("EAWC_4_1");
      sentAwcHttp = true;
      if(digitalRead(4) == LOW){//low sensor in the water
        Serial.println("Sensor 2 low level sensor is in the water");
        //waterChange = true;
        digitalWrite(2, 1);  //turn sump pump on
        Serial.println("Pump 3 Sump pump is running");
        if(!wswAwc2){
          wswAwc2 = true;
          //_atoAwcUtil->webSerialWrite("EAWC_4_2");
        }
      }else{//water is emptied now
        Serial.println("AWC water is emptied");
        digitalWrite(2, 0);  //turn sump pump off
        Serial.println("Pump 3 Sump pump is stopped");
        if(!wswAwc3){
          wswAwc3 = true;
         // _atoAwcUtil->webSerialWrite("EAWC_4_3");
        }
       waterEmptied = true;
       Serial.println("Marked water as emptied");
        waterFilled = false;
      }//emptied water
    }///////////////// EMPTY WATER DONE /////////////////////
    if(waterEmptied == true && waterFilled == false){
      if(digitalRead(3) == HIGH){//normal sensor out of water
        Serial.println("Sensor 1 normal-level is out of water");
        digitalWrite(1, 1);  // new water pump on
        Serial.println("Pump 2 new-water pump is running");
        float empty = _atoAwcUtil->recordRun("awc_fill",true);
        if(!wswAwc4){
          wswAwc4 = true;
          //_atoAwcUtil->webSerialWrite("EAWC_4_4");
        }
      }else{
        Serial.println("AWC water is filled back up");
        digitalWrite(1, 0);//turn off fill pump
        Serial.println("Pump 2 new-water pump is stopped");
        float full = _atoAwcUtil->recordRun("awc_fill",false);
        if(!wswAwc5){
          wswAwc5 = true;
           //_atoAwcUtil->webSerialWrite("EAWC_4_5");
        }
        waterFilled = true;
        waterEmptied = false;
        waterChange = false; 
        executingAwc = false;
        sentAwcHttp = false;
        wswAwc2 = false;
        wswAwc3 = false;
        wswAwc4 = false;
        wswAwc5 = false;
       Serial.println("Marked waterFilled T, waterChange F, and executingAwc F");
      }
    } 
  } 
}//end of water change and water emptied true

/////////////////////////////////////////////////////////////////////////////////////
//                        end of doWaterChange
/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
/////////////////////////////////////////////
float Awc::calcPadded(float percent, unsigned long value){
  ;
}
    
bool Awc::getWaterChange(){
  return waterChange;
}
void Awc::setWaterChange(bool val){
  waterChange = val;
}
bool Awc::getExecuteAwc(){
  return executeAwc;
}
void Awc::setExecuteAwc(bool val){
  executeAwc = val;
}
bool Awc::getExecutedAwc(){
  return executedAwc;
}
void Awc::setExecutedAwc(bool val){
  executedAwc = val;
}
bool Awc::getExecutingAwc(){
  return executingAwc;
}
void Awc::setExecutingAwc(bool val){
  executingAwc = val;
}
bool Awc::getSentAwcHttp(){
  return sentAwcHttp;
}
void Awc::setSentAwcHttp(bool val){
  sentAwcHttp = val;
}

bool Awc::getWaterEmptied(){
  return waterEmptied;
}
void Awc::setWaterEmptied(bool val){
  waterEmptied = val;
}
