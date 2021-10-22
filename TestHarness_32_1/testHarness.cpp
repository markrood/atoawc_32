#include "testHarness.h"

  Testharness::Testharness(){
    ;
  }

  //int digRead(int pin){
  int Testharness::digRead(int pin){
        int ret = 999;
      if(pin == 2){  //pin 2 is normal water level
        startTimer("evap", _resetEvapTimer);
        int retVal = action("evap");
        if(retVal ==1){
          _resetEvapTimer = true;
          //System.out.println("resetting resetEvapTimer");
          Serial.println("resetting resetEvapTimer");
          _pin2 = 1;
          ret = _pin2;
        }
        ret = _pin2;
      }else if(pin == 22){  //pin 22 in highwater
        ret =  _pin22;
      }else if(pin == 0){  //pin 0 low water
        ret = _pin0;
      }
      //TODO
      return ret;
    }

  //void digWrite(int pin, int value){
  void Testharness::digWrite(int pin, int value){
      if(pin == 12){  //ro pump
        _pin12 = value;
        if(_pin12 == 0){
          pump("ro",0);
        }else{
          pump("ro",1);
          //System.out.println("Ro pump on");
          Serial.println("Ro pump is on");
        }
      }else if(pin == 14){  //sump pump
        _pin14 = value;
        if(_pin14 == 0){
            pump("sump",0);
            //System.out.println("resetting resetSumpPumpTimer");
            Serial.println("resetting resetSumpPumpTimer");
            _resetSumpPumpTimer = true;
          }else{
            pump("sump",1);
            //System.out.println("Sump pump on");
            Serial.println("Sump pump is on");
          }
      }else if(pin == 27){  //fill pump
        _pin27 = value;
        if(_pin27 == 0){
            pump("fill",0);
            _resetFillPumpTimer = true;
            _pin2 = 0;
            //System.out.println("low sensor in the water");
            Serial.println("low sensor is in the water");
            _pin0 = 0;
            //System.out.println("normal sesnor in the water");
            Serial.println("normal sensor is in the water");
            //System.out.println("resetting resetFillPumpTimer");
            Serial.println("resetting resetFillPumptimer");
          }else{
            pump("fill",1);
            //System.out.println("New pump on");
            Serial.println("New Pump is on");
          }
      }
    }
    
    //int anaRead(int pin){
    int Testharness::anaRead(int pin){
      int retVal = 999;

      if(pin == 34){  //analog 0 leak
        retVal =  _pin34;
      }
      return retVal;
    }

    void Testharness::pump(String pumpName, int value){
      if(pumpName == "ro" && value == 1){
        //ro pump is running
        //System.out.println("Ro pump is running");
        Serial.println("Ro pump is running");
        //pin2 = 1;;
        startTimer("ro", _resetRoPumpTimer); //12,14,27
        int retVal = action("ro");
        if(retVal == 4){
          _resetRoPumpTimer = true;
          Serial.println("resetting resetEvapTimer");
          //System.out.println("resetting resetRoPumpTimer");
          _pin2 = 0;
          _pin0 = 0;
          //System.out.println("pin 2 is "+pin2);
          Serial.print("_pin2 is ");
          Serial.println(_pin2);
          //System.out.println("pin0 is "+pin0);
          Serial.print("_pin0 is ");
          Serial.println(_pin0);
        }
      }
        if(pumpName == "sump" && value == 1){
          //ro pump is running
          //System.out.println("Sump pump is running");
          Serial.println("Sump pump is running");
          //TODO pin2 = 1;;
          startTimer("sump", _resetSumpPumpTimer);
          int retVal = action("sump");
          if(retVal == 2){
            _pin2 = 1;
            _pin0 = 1;
            //System.out.println("pin 2 is "+pin2);
            Serial.print("_pin2 is ");
            Serial.println(_pin2);
            //System.out.println("pin0 is "+pin0);
            Serial.print("_pin0 is ");
            Serial.println(_pin0);
          }
        }
        if(pumpName == "fill" && value == 1){
          //ro pump is running
         // System.out.println("Fill pump is running");
         Serial.println("fill pump is running");
          //TODO pin2 = 1;;
          startTimer("fill", _resetFillPumpTimer);
          int retVal = action("fill");
          if(retVal == 3){
            _pin2 = 0;
            _pin0 = 0;
            //System.out.println("pin 2 is "+pin2);
            //System.out.println("pin0 is "+pin0);
          }
        }     
    }
    //void startTimer(String timerName,boolean reset){
    void Testharness::startTimer(String timerName,bool reset){
        if(timerName == "evap"){
          //Serial.println("evap timer started");
          if(_evapFirst){
            //evapTime = millis();
            //evapTime = System.currentTimeMillis();
            _evapTime = millis();
            _evapFirst = false;
          }
          if(reset){
            //evapTime = millis();
//            evapTime =System.currentTimeMillis();
            _evapTime = millis();
            _resetEvapTimer = false;
          }
        }
        if(timerName == "ro"){
            if(_roFirst){
              //evapTime = millis();
              //roTime = System.currentTimeMillis();
              _roTime = millis();
              _roFirst = false;
            }
            if(reset){
              //evapTime = millis();
              //roTime = System.currentTimeMillis();
              _roTime = millis();
              //roTime = millis();
              _resetRoPumpTimer = false;
            }
          }
        if(timerName == "sump"){
            if(_sumpFirst){
              //evapTime = millis();
              //sumpTime = System.currentTimeMillis();
              _sumpTime = millis();
              _sumpFirst = false;
            }
            if(reset){
              //evapTime = millis();
              //sumpTime =System.currentTimeMillis();
              _sumpTime = millis();
              _resetSumpPumpTimer = false;
            }
          }
        if(timerName == "fill"){
            if(_fillFirst){
              //evapTime = millis();
              //fillTime = System.currentTimeMillis();
              _fillTime = millis();
              _fillFirst = false;
            }
            if(reset){
              //evapTime = millis();
              //fillTime =System.currentTimeMillis();
              _fillTime = millis();
              _resetFillPumpTimer = false;
            }
          }
 /*       if(timerName == "awc"){
            if(awcFirst){
              //evapTime = millis();
              awcTime = System.currentTimeMillis();
              awcFirst = false;
            }
            if(reset){
              //evapTime = millis();
              awcTime =System.currentTimeMillis();
              resetAwcTimer = false;
            }
          }*/
      }
      //int Testharness::action(String actName){
    int Testharness::action(String actName){
        int retVal = 0;
        //long nowEvapTime = System.currentTimeMillis() - _evapTime;
        long nowEvapTime = millis() - _evapTime;
        if(actName == "evap"){
          if(nowEvapTime > EVAP_TIME){
            _pin2 = 1; 
            _evapTime = 0;
            retVal = 1;
          }
        }
        //long nowRoTime = System.currentTimeMillis() - _roTime;
        long nowRoTime = millis() - _roTime;
        if(actName == "ro"){
          if(nowRoTime > RO_PUMP_RUN){
            _pin2 = 1; 
            _roTime = 0;
            Serial.println("return 4 for ro");
            retVal = 4;
          }
        }
        //long nowSumpTime = System.currentTimeMillis() - _sumpTime;
        long nowSumpTime = millis() - _sumpTime;
        if(actName == "sump"){
          if(nowSumpTime > SUMP_PUMP_RUN){
            _pin2 = 1; 
            _sumpTime = 0;
            retVal = 2;
          }
        }
        //long nowFillTime = System.currentTimeMillis() - _fillTime;
        long nowFillTime = millis() - _fillTime;
        if(actName == "fill"){
          if(nowFillTime > FILL_PUMP_RUN){
            _pin2 = 0; 
            _fillTime = 0;
            retVal = 3;
          }
        }
  /*      long nowAwcTime = System.currentTimeMillis() - awcTime;
        if(actName == "awc"){
          if(nowAwcTime > AWC_RUN){
            pin2 = 0; 
            awcTime = 0;
            retVal = 5;
          }
        }*/
        return retVal;
      }
    
