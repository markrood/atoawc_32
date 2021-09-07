/*
  ATO_.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef AWC_h
#define AWC__h

#include "Arduino.h"
//#include "ESPFlash.h"
#include <FS.h>
#include <atoawc_util.h>

class Awc
{
  public:
    Awc(AtoAwcUtil *atoAwcUtil);
    void doWaterChange();
    bool getWaterChange();
    void setWaterChange(bool val);
    bool getExecuteAwc();
    void setExecuteAwc(bool val);
    bool getExecutedAwc();
    void setExecutedAwc(bool val);
    bool getExecutingAwc();
    void setExecutingAwc(bool val);
    bool getSentAwcHttp();
    void setWaterEmptied(bool val);
    bool getWaterEmptied();
    bool wswAwc2 = false;
    bool wswAwc3 = false;
    bool wswAwc4 = false;
    bool wswAwc5 = false;
    void setSentAwcHttp(bool val);
    float calcPadded(float percent, unsigned long value);
    bool itRan = false;
   
  private:
    AtoAwcUtil *_atoAwcUtil;
    bool waterChange = false;
    bool executeAwc = false;
    bool executedAwc = false;
    bool executingAwc = false;
    bool sentAwcHttp = false;
    bool waterEmptied = false;
    bool waterFilled = true;
    float startFill = 0;
    float startEmpty = 0;
    float runningFill = 0;
    float runningEmpty = 0;
    bool firstTime = true;



};

#endif
