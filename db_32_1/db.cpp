#include "db.h";

    Db::Db(){
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      
      //Comment or pass false value when WiFi reconnection will control by your code or third party library
      Firebase.reconnectWiFi(true);
      Firebase.setDoubleDigits(5);
    }
    
    bool Db::putFloat(String nam, float val){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.setFloat(fbdo, nam, val);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    bool Db::putString(String nam, String val){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Serial.println("made it right before the write to firebase");
        Serial.println(nam);
        Serial.println(val);
        Firebase.setString(fbdo, nam, val);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    bool Db::putInt(String nam, int val){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.setInt(fbdo, nam, val);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    bool Db::putBool(String nam, bool val){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.setBool(fbdo, nam, val);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    float Db::getFloat(String nam){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.getFloat(fbdo, nam);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    String Db::getStr(String nam){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.getString(fbdo, nam);
      // handle error
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    int Db::getInt(String nam){
      bool retVal = true;
    // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.getInt(fbdo, nam);
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
    bool Db::getBool(String nam){
      bool retVal = true;
      // set value
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        Firebase.getBool(fbdo, nam);
      }else{        
        Serial.println(fbdo.errorReason().c_str());
        retVal = false;
      }
    }
    
