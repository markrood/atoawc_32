/*
  Uset to get date and time with timezone and hopefully
  daylight savings time.
*/
#ifndef DATETIME_32_h
#define DATETIME_32_h

#include <WiFi.h>
#include "time.h"
#include "Arduino.h"


class Datetime
{
  public:
    Datetime(int offset,bool dst);
    void updateTime();
    //unsigned long getEpochTime();
    int getMonthDay();
    int getCurrentMonth();
    String getCurrentMonthName();
    int getCurrentYear();
    String getFormattedTime_();
    int getCurrentDay();
    int getCurrentHour();
    int getCurrentMinute();
    int getCurrentSecond();
    String getWeekDay();
    String getCurrentDate();
    
  private:
    //NTPClient *_timeClient;
    //WiFiUDP ntpUDP;
    const char* ntpServer = "pool.ntp.org";
    //bool daylightSavingsTime = false;
    //Week Days
    String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    //Month names
    String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    unsigned long epochTime;
    //struct tm *ptm;
    struct tm timeinfo;
    int monthDay;
    int currentMonth;
    String currentMonthName;
    int currentYear;
    String formattedTime;
    int currentDay;
    int currentHour;
    int currentMinute;
    int currentSecond;
    String dayWeek;
    String currentDate;
    ///////////////////////////////////
    
};
#endif
