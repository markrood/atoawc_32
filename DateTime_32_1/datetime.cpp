#include "datetime.h"

  Datetime::Datetime(int offset,bool dst){
    int daylightOffset_sec = 0;
    if(dst){
      daylightOffset_sec = 3600;
    }
    configTime(offset, daylightOffset_sec, ntpServer);
    //Serial.println(timeinfo.tm_mon);
  }

  void Datetime::updateTime(){
    //struct tm timeinfo;
    bool timeUpdate = false;
    while(!timeUpdate){
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        Serial.println("THIS IS AN ERROR WE ARE LOOPING UNTIL GET TIME");
        timeUpdate = false;
        delay(30000);
      }else{
        timeUpdate = true;
      }
    }
 /*    _timeClient->begin();
    _timeClient->update();
    _timeClient->forceUpdate();
    epochTime =_timeClient->getEpochTime();
    monthDay = ptm->tm_mday;
    currentMonth = ptm->tm_mon+1;
    currentMonthName = months[getCurrentMonth()-1];*/
    currentYear = timeinfo.tm_year+1900;
    currentMonth = timeinfo.tm_mon+1;
    currentMonthName = months[getCurrentMonth()-1];
    dayWeek = weekDays[timeinfo.tm_wday];
    currentDay = timeinfo.tm_mday;
    currentHour = timeinfo.tm_hour;
    currentMinute = timeinfo.tm_min;
    currentSecond = timeinfo.tm_sec;
/*    formattedTime = _timeClient->getFormattedTime();
    currentHour = _timeClient->getHours();
    currentMinute = _timeClient->getMinutes();
    currentSecond = _timeClient->getSeconds();
    weekDay = weekDays[_timeClient->getDay()];
    currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);*/
  }


    int Datetime::getMonthDay(){
      return monthDay;
    }


    int Datetime::getCurrentMonth(){
      return currentMonth;
    }

    String Datetime::getCurrentMonthName(){
      return currentMonthName;
    }

    
    int Datetime::getCurrentYear(){
      return currentYear;
    }

    String Datetime::getFormattedTime_(){
      return formattedTime;
    }

    int Datetime::getCurrentDay(){
      return currentDay;
    }


    int Datetime::getCurrentHour(){
      return currentHour;
    }

    int Datetime::getCurrentMinute(){
      return currentMinute;
    }

    int Datetime::getCurrentSecond(){
      return currentSecond;
    }

    String Datetime::getWeekDay(){
      return dayWeek;
    }

    String Datetime::getCurrentDate(){
      return currentDate;
    }
