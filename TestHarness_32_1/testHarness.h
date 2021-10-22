#ifndef TESTHARNESS_32_h
#define TESTHARNESS_32_h

#define _HIGH 1
#define _LOW 0

//#define EVAP_TIME 300000
  /*#define EVAP_TIME 300000
  #define SUMP_PUMP_RUN 30000
  #define NEW_PUMP_RUN 30000*/
/*  //int EVAP_TIME = 300000;
  #define EVAP_TIME 300000
  //int SUMP_PUMP_RUN = 30000;
  #define SUMP_PUMP_RUN 30000
  //int NEW_PUMP_RUN = 30000;
  #define NEW_PUMP_RUN 30000
  //int RO_PUMP_RUN = 10000;
  #define RO_PUMP_RUN 10000
  //int FILL_PUMP_RUN = 10000;
  #define FILL_PUMP_RUN 10000*/
  //int AWC_RUN = 1800000; //TODO change back to 12 hours
  

  

/*    //boolean awcFirst = true;
    bool awcFirst = true;
    long awcTime = 0;
    //boolean resetAwcTimer = false;
    bool resetAwcTimer = false; */ 
    

#include "Arduino.h"

class Testharness
{
  public:
    Testharness();
    int digRead(int pin);
    void digWrite(int pin, int value);
    int anaRead(int pin);
    void pump(String pumpName, int value);
    void startTimer(String timerName, bool reset);
    int action(String actName); 


    private:
    int _pin0 = 0;   //low sensor
    int _pin2 = 0;   //normal sensor
    int _pin12 = 0;  //ro pump
    int _pin14 = 0;  //sump pump
    int _pin22 = 1;  //High sensor
    int _pin27 = 0;  //new-water pump
    int _pin34 = 0;  //leak analog
    //boolean evapFirst = true;
    bool _evapFirst = true;
    long _evapTime = 0;
    //boolean resetEvapTimer = false;
    bool _resetEvapTimer = false;
    //
   // boolean roFirst = true;
    bool _roFirst = true;
    long _roTime = 0;
    //boolean resetRoPumpTimer = false;
    bool _resetRoPumpTimer = false;
    //
    //boolean sumpFirst = true;
    bool _sumpFirst = true;
    long _sumpTime = 0;
    //boolean resetSumpPumpTimer = false;
    bool _resetSumpPumpTimer = false;
    //
    //boolean fillFirst = true;
    bool _fillFirst = true;
    long _fillTime = 0;
    //boolean resetFillPumpTimer = false;
    bool _resetFillPumpTimer = false;
    int EVAP_TIME = 300000;
    //#define EVAP_TIME 300000
    int SUMP_PUMP_RUN = 30000;
    //#define SUMP_PUMP_RUN 30000
    int NEW_PUMP_RUN = 30000;
    //#define NEW_PUMP_RUN 30000
    int RO_PUMP_RUN = 10000;
    //#define RO_PUMP_RUN 10000
    int FILL_PUMP_RUN = 10000;
    //#define FILL_PUMP_RUN 10000
       
};

#endif
