#include "ato.h"

Ato::Ato(AtoAwcUtil *atoAwcUtil)
{
  _atoAwcUtil = atoAwcUtil;

}

//////////////////////////////////////////////////////////////////
//
//  beginAto()
//
//  sets up pins
//
/////////////////////////////////////////////////////////////////
void Ato::beginAto(){
  pinMode(3, INPUT);  //normal sensor
  pinMode(4, INPUT);  //low sensor
  pinMode(5, INPUT);  //high sensor
  pinMode(0, OUTPUT); //RO pump
  pinMode(1, OUTPUT); //RO pump
  pinMode(2, OUTPUT); //RO pump
  pinMode(8, OUTPUT); //norm light
  pinMode(9, OUTPUT);  //low light
  pinMode(10, OUTPUT);  //high light
}


//////////////////////////////////////////////////////////////
//                                                          //
//   Function: doAto()                                      //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  runs auto top off (ato)                  //
//                                                          //
//////////////////////////////////////////////////////////////
void Ato::doAto(){
  //didAto = false;
  if(waterChange == false){
    sentAtoHttp = true;
    executeAto = true;
    Serial.println("Executing ATO");
      if(digitalRead(3) == HIGH){  // normal level sensor out of water
        if(executeAto){
          itRan = true;
          executingAto = true;
          startFill = millis();
          executeAto = false;
          
        }
        Serial.println("Sensor1 normal level is out of water");
        digitalWrite(0, HIGH);  //run ato pump
        Serial.println("Pump 1 RO water pump is running");
        float empty = _atoAwcUtil->recordRun("ato",true);
        if(!sentWebSerialWrite){
          //_atoAwcUtil->webSerialWrite("EATO_5_1");
          sentWebSerialWrite = true;
          //didAto = true;
        }
    }else{
      Serial.println("Ato water is full again!");
      if(itRan){
        digitalWrite(0, LOW);//turn off RO pump
        float full = _atoAwcUtil->recordRun("ato",false);
        runningFill = millis();
        //_atoAwcUtil->webSerialWrite("EATO_5_2");
        Serial.println("Pump 1 Ro pump is off");
        executingAto = false;
        sentAtoHttp = false;
        executeAto = true;
        float elapse = runningFill-startFill;
        startFill = 0;
        runningFill = 0;
        executingAto = _atoAwcUtil->checkStoreRun("ato_fill", elapse);
      }
        executingAto = false;
        sentAtoHttp = false;
        executeAto = true;
        startFill = 0;
        runningFill = 0;
        itRan = false;
        sentWebSerialWrite = false;
        
    }

    
  }
    
} //auto top off

//////////////////////////////////////////////////////////////
//                                                          //
//   checkLeaks                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for leaks                         //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkLeaks(){
  Serial.println("in checkLeaks");
  int lklevel = 0;
  String llevel = _atoAwcUtil->readFile(SPIFFS, "/leaklevel.txt");
  if(llevel == "moister"){
    lklevel = 500;
  }else if(llevel == "damp"){
    lklevel = 700;
  }else if(llevel == "wet"){
    lklevel = 900;
  }
  //int leaklevel = llevel.toInt();
  resVal = analogRead(resPin);
//CUT
Serial.print("analog value for leak is ");
Serial.println(resVal);
Serial.print("level for wetness is ");
Serial.println(lklevel);
  if(resVal >=lklevel){
     digitalWrite(6, HIGH);//turn off pump Pin 6
     //int status = sendHttp("Leak"); 
     return true;
  }else{
    digitalWrite(6, LOW);
    return false;
  }
}
//////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////
//                                                          //
//   checkHighwater                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for high water                        //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkHighWater(){
  //if pin x is high

  int  val = digitalRead(5);//TODO if high water pin is low
  if(!val){
     //int status = sendHttp("HighWater"); 
     return true;
  }else{
    return false;
  }
}
//////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////
//                                                          //
//   checkLowwater                                             //
//                                                          //
//   input: none                                            //
//                                                          //
//   output:  bool                                          //
//                                                          //
//   description:  checks for high water                        //
//                                                          //
//////////////////////////////////////////////////////////////
bool Ato::checkLowWater(){
  //if pin x is high

  int  val = digitalRead(4);//TODO if low water pin is high
  if(val){
     //int status = sendHttp("LowWater"); 
     return true;
  }else{
    return false;
  }
}

void Ato::doWaterLights(){
      int hw = digitalRead(3);  //yellow mcp.out8
    //Serial.print("Water Normal sensor is ");
    //Serial.println(hw);
    if(hw == 0){
      digitalWrite(8,HIGH);
    }else{
      digitalWrite(8,LOW);
    }
    
    int lw = digitalRead(4);  //purple mcp.out9
    //Serial.print("Water Low sensor is ");
    //Serial.println(lw);
     if(lw == 0){
      digitalWrite(9,HIGH);
    }else{
      digitalWrite(9,LOW);
    }
   
    int ehw = digitalRead(5);  //red mcp.out10
    //Serial.print("Extreme Water High sensor is ");
    //Serial.println(ehw);
    if(ehw == 0){
      digitalWrite(10,HIGH);
    }else{
      digitalWrite(10,LOW);
    }

}
bool Ato::getWaterChange(){
  return waterChange;
}
void Ato::setWaterChange(bool val){
  waterChange = val;
}
bool Ato::getExecuteAto(){
  return executeAto;
}
void Ato::setExecuteAto(bool val){
  executeAto = val;
}
bool Ato::getExecutedAto(){
  return executedAto;
}
void Ato::setExecutedAto(bool val){
  executedAto = val;
}
bool Ato::getExecutingAto(){
  return executingAto;
}
void Ato::setExecutingAto(bool val){
  executingAto = val;
}
bool Ato::getSentAtoHttp(){
  return sentAtoHttp;
}
void Ato::setSentAtoHttp(bool val){
  sentAtoHttp = val;
}
bool Ato::getItRan(){
  return itRan;
}
void Ato::setItRan(bool val){
  itRan = val;
}
