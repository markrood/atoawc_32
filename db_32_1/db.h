/*
  db.h - Library supporting atoawc.
  Created by Mark Rood, Markch 9, 2021.
  Not Released into the public domain.
*/
#ifndef DB_h
#define DB_h

// Set these t.
#define FIREBASE_HOST "atoawc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "eVZrxj9rlCePsyurSvcjlx8KrOQgZig243JraiEu"


#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


/*
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"*/

/* 2. Define the API Key */
#define API_KEY "API_KEY"



class Db
{
  public:
    Db();
    bool putFloat(String nam, float val);
    bool putString(String nam, String val);
    bool putInt(String nam, int val);
    bool putBool(String nam, bool val);
    float getFloat(String nam);
    String getStr(String nam);
    int getInt(String nam);
    bool getBool(String nam);

  private:
    unsigned long sendDataPrevMillis = 0;
    unsigned long count = 0;
    FirebaseData fbdo;


};
#endif
