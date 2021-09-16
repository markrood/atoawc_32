/*
  Dosing.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef DOSING_h
#define DOSING_h

#include "Arduino.h"
#include "atoawc_util.h";
#include <WiFiClientSecure.h>
//#include <WiFiManager.h>

class Dosing
{
  public:
    Dosing(AtoAwcUtil *atoAwcUtil,WiFiClient client);
    void sendJson();
    void calibrate();
    void dose(String pump);
    int calculateDosingLength(int cal, int amount);
    void setCkBox1(bool value);
    bool getCkBox1();
    void setCkBox2(bool value);
    bool getCkBox2();
    void setCkBox3(bool value);
    bool getCkBox3();
    bool getCalibrating();
    void setCalibrating(bool cal);

  private:
//  bool atoRunning;
    //WiFiManager wifiManager;
    AtoAwcUtil *_atoAwcUtil;
    WiFiClient _client;
    int sensorValue0 = 0;    //sensor value, I'm usingg 0/1 button state
    int sensorValue1 = 0;        
    int sensorValue2 = 0;        
    int sensorValue3 = 0;  
    String slider1;
    String slider2;
    String slider3;
    String chkboxValue1;
    String chkboxValue2;
    String chkboxValue3;
    bool ckBox1 = false;
    bool ckBox2 = false;
    bool ckBox3 = false;
    bool calibrating = false;
    int oneHundredmlBlueTime = 0;
    int oneHundredmlGreenTime = 0;
    int oneHundredmlYellowTime = 0;
    bool blueStarted = false;
    bool greenStarted = false;
    bool yellowStarted = false;
    bool blueRan = false;
    bool greenRan = false;
    bool yellowRan = false;

  
};

#endif
