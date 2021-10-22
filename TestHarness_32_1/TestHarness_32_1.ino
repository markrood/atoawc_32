#include "testHarness.h"

Testharness  *_testharness;
    int count;
    int atoCount;
    int awcCount;
    bool roPumpRan;
    bool  awc;
    bool ato;
    bool emptied;
    bool filled;
    bool resetAwcTimer;

void setup() {
  Serial.begin(115200);
    _testharness = new Testharness();
    count = 0;
    atoCount = 0;
    awcCount = 0;
    roPumpRan = false;
    awc = false;
    ato = false;
    emptied = false;
    filled = false;
    resetAwcTimer = false;

}

void loop() {
  // put your main code here, to run repeatedly:
      // this just evaps water
      int pin = _testharness->digRead(2);
      //Serial.print("Pin2 is ");
      //Serial.println(pin);
      if( pin== 1) {
        count++;
      }
      if(count == 5000) {
       // System.out.print(pin+", ");
       Serial.print(pin+", ");
        // testharness.setPin2(0);
      }
      // now lets turn on ro pump because of evap
 /*     try {
        TimeUnit.SECONDS.sleep(1);
      }catch(Exception e) {
        System.out.println(e);
      }*/
      delay(1000);
      int normal = _testharness->digRead(2);
      if(normal == 1 && awc == false) {
        ato = true;
        //need to run ro pump
        _testharness->digWrite(12, 1);
        //System.out.println("RO pump is running");
        Serial.println("RO pump is running");
        roPumpRan = true;
      }else {
        _testharness->digWrite(12, 0);
        if(roPumpRan) {
          //System.out.println("RO pump is turned OFF");
          Serial.println("RO pump is turned OFF");
          roPumpRan = false;
          ato = false;
          atoCount++;
          //System.out.println(atoCount);
          Serial.println(atoCount);
        }
      }
      //when do i run awc
      _testharness->startTimer("awc", resetAwcTimer);
      resetAwcTimer = false;
      int retVal = _testharness->action("awc");
      if(retVal == 5) {
        resetAwcTimer = true;
      }
      if((retVal ==  5 && ato == false) || awc == true)  {
        if(!emptied) {
          
          awc = true;
          //turn on sump
          _testharness->digWrite(14,1);
          //when emptied
          if(_testharness->digRead(0) == 1) {
            _testharness->digWrite(14, 0);  //turn off sump
            emptied = true;
            //System.out.println("emptied = "+emptied);
            Serial.println("emptied = "+emptied);
          }
        }
        //fill back up
        if(emptied && !filled) {
          if(_testharness->digRead(2) == 1) { // normal out of water
            _testharness->digWrite(27, 1); //turn new pump on
          }else {
            _testharness->digWrite(27, 0);  //turn new pump off
            emptied = false;
            filled = false;
            //resetAwcTimer = true;
            awc = false;
            awcCount++;
            //System.out.println("Awc Count is "+awcCount);
            Serial.println("Awc Count is "+awcCount);
          }
        }
      }

}
