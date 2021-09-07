/*
  ATO_.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef ATO__h
#define ATO__h

#include <atoawc_util.h>


class Ato
{
  
  public:
    Ato(AtoAwcUtil *atoAwcUtil);
    void doAto();
    bool checkLowWater();
    bool checkHighWater();
    bool checkLeaks();
    
    bool getWaterChange();
    void setWaterChange(bool val);
    bool getExecuteAto();
    void setExecuteAto(bool val);
    bool getExecutedAto();
    void setExecutedAto(bool val);
    bool getExecutingAto();
    void setExecutingAto(bool val);
    bool getSentAtoHttp();
    void setSentAtoHttp(bool val);
    bool getItRan();
    void setItRan(bool val);
    //new sensor lights
    void doWaterLights();
    void beginAto();


  private:
    AtoAwcUtil *_atoAwcUtil;
    AsyncWebServer *server;
    bool waterChange = false;
    bool executeAto = false;
    bool executedAto = false;
    bool executingAto = false;
    bool sentAtoHttp = false;
    bool sentWebSerialWrite = false;
    //bool didAto = false;
    float startFill = 0;
    float runningFill = 0;
    int resVal = 0;
     const int resPin = A0;
     bool itRan = false;
};

#endif
