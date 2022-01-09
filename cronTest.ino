/*
 * CronAlarmsExample.ino
 *
 * This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
 * and simulates turning lights on at night and off in the morning
 * A weekly timer is set for Saturdays at 8:30:30
 *
 * A timer is called every 15 seconds
 * Another timer is called once only after 10 seconds
 *
 * At startup the time is set to Jan 1 2011  8:29 am
 */
#ifdef ESP8266
  #include <ESP8266WiFi.h> // in order to use WiFi.mode(WIFI_OFF)
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include <time.h>                       // time() ctime()
#ifdef ESP8266
#include <sys/time.h>                   // struct timeval
#endif
#include "CronAlarms.h"
#include "datetime.h"
CronId id;
Datetime *dateTime;
const char* ssid     = "RoodRouter";
const char* password = "";


long doAto();
long doAwcEmpty();
long doAwcFill();

int wetness = 200;
int leakDetect = 0;

/////////////////////db stuff /////////////////////////////
int atoLastAmount = 999;
int awcFillLastAmount = 999;
int awcEmptyLastAmount = 999;
////////////////////////////////////////////////////////

void TenMinute();
  void ThirtyMinute();
  void OneHour();
  void TwoHour();
  void ThreeHour();
  void FourHour();
  void EightHour();
  void Midnight();
  void OneAm();
  void TwoAm();
  void ThreeAm() ;
  void FourAm();
  void FiveAm();
  void SixAm();
  void SevenAm() ;
  void EightAm();
  void NineAm() ;
  void TenAm() ;
  void ElevenAm();
  void Noon();
  void OnePm() ;
  void TwoPm();
  void ThreePm();
  void FourPm();
  void FivePm();
  void SixPm();
  void SevenPm() ;
  void EightPm() ;
  void NinePm();
  void TenPm() ;
  void ElevenPm() ;

bool ato = false;
bool atoRunning = false;
long atoRunTimeSec = 0;
long beginAtoTime = 0;
bool awcEmpty = false;
bool awcEmptyRunning = false;
long awcEmptyRunTimeSec = 0;
bool awcEmptyRan = false;
long beginAwcEmptyTime = 0;
bool awcFill = false;
bool awcFillRunning = false;
long awcFillRunTimeSec = 0;
long beginAwcFillTime = 0;
long atoRet = 0;
long awcEmptyRet = 0;
long awcFillRer = 0;
long count = 0;

bool restart = false;
void setup() {
//#if defined(ESP8266) || defined(ESP32)
//  WiFi.mode(WIFI_OFF); // disconect wifi to prevent NTP setting the time
//#endif
  Serial.begin(115200);
////////////////////////////////////////////////wifi ////////////////////////////
//TODO make wifimanager
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  delay(1000);
/////////////////////////////////////////////////////////////
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("Starting setup...");
  dateTime = new Datetime(-21600,false);
  dateTime->updateTime();
  Serial.print("yyyyyyyyyyyyyear is: ");
  Serial.println(dateTime->getCurrentHour());
  struct tm tm_newtime; // set time to Saturday 8:29:00am Jan 1 2011
  tm_newtime.tm_year = dateTime->getCurrentYear() - 1900;
  tm_newtime.tm_mon = dateTime->getCurrentMonth() - 1;
  tm_newtime.tm_mday = dateTime->getCurrentDay();
  tm_newtime.tm_hour = dateTime->getCurrentHour()-6;
  tm_newtime.tm_min = dateTime->getCurrentMinute();
  tm_newtime.tm_sec = 0;
  tm_newtime.tm_isdst = 0;
//#if defined(ESP8266) || defined(ESP32)
//  WiFi.mode(WIFI_OFF); // disconect wifi to prevent NTP setting the time
//#endif
#if defined(ESP8266) || defined(ESP32)
  timeval tv = { mktime(&tm_newtime), 0 };
  settimeofday(&tv, nullptr);
#elif defined(__AVR__)
  set_zone(0);
  set_dst(0);
  set_system_time( mktime(&tm_newtime) );
#endif

  // create the alarms, to trigger at specific times
  //Cron.create("0 30 8 * * *", MorningAlarm, false);  // 8:30am every day
  //Cron.create("0 45 17 * * *", EveningAlarm, false); // 5:45pm every day
 // Cron.create("30 30 8 * * 6", WeeklyAlarm, false);  // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  //Cron.create("*/15 * * * * *", Repeats, false);           // timer for every 15 seconds
  //id = Cron.create("*/2 * * * * *", Repeats2, false);      // timer for every 2 seconds
  //Cron.create("*/10 * * * * *", OnceOnly, true);           // called once after 10 seconds
  Cron.create("0 */10 * * * *", TenMinute, false);
  Cron.create("0 */30 * * * *", ThirtyMinute, false);
  Cron.create("0 0 */1 * * *", OneHour, false);
  Cron.create("1 0 */2 * * *", TwoHour, false);
  Cron.create("1 0 */3 * * *", ThreeHour, false);
  Cron.create("1 0 */4 * * *", FourHour, false);
  Cron.create("1 0 */8 * * *", EightHour, false);
  Cron.create("2 0 0 * * *", Midnight, false);
  Cron.create("2 0 1 * * *", OneAm, false);
  Cron.create("2 0 2 * * *", TwoAm, false);
  Cron.create("2 0 3 * * *", ThreeAm, false);
  Cron.create("2 0 4 * * *", FourAm, false);
  Cron.create("2 0 5 * * *", FiveAm, false);
  Cron.create("2 0 6 * * *", SixAm, false);
  Cron.create("2 0 7 * * *", SevenAm, false);
  Cron.create("2 0 8 * * *", EightAm, false);
  Cron.create("2 0 9 * * *", NineAm, false);
  Cron.create("2 0 10 * * *", TenAm, false);
  Cron.create("2 0 11 * * *", ElevenAm, false);
  Cron.create("2 0 12 * * *", Noon, false);
  Cron.create("2 0 13 * * *", OnePm, false);
  Cron.create("2 0 14 * * *", TwoPm, false);
  Cron.create("2 0 15 * * *", ThreePm, false);
  Cron.create("2 0 16 * * *", FourPm, false);
  Cron.create("2 0 17 * * *", FivePm, false);
  Cron.create("2 0 18 * * *", SixPm, false);
  Cron.create("2 0 19 * * *", SevenPm, false);
  Cron.create("2 0 20 * * *", EightPm, false);
  Cron.create("2 0 21 * * *", NinePm, false);
  Cron.create("2 0 22 * * *", TenPm, false);
  Cron.create("2 0 23 * * *", ElevenPm, false);
    time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
 Serial.println("Ending setup...");

  pinMode(2,INPUT);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(5,OUTPUT);
  digitalWrite(5,LOW);
  pinMode(26,INPUT);
  pinMode(14,OUTPUT);
  digitalWrite(14,LOW);
  pinMode(34,INPUT);
  pinMode(15,OUTPUT);
  digitalWrite(15,LOW);
  pinMode(27,OUTPUT);
  digitalWrite(27,LOW);
  
}

void loop() {
if(WiFi.status() != WL_CONNECTED){
  ESP.restart();
}
count++;
if(count >= 60){
  Serial.print(".");
  count = 0;
}
#ifdef __AVR__
  system_tick(); // must be implemented at 1Hz
#endif
  time_t tnow = time(nullptr);
  //Serial.println(asctime(gmtime(&tnow)));
  Cron.delay(500);// if the loop has nothing else to do, delay in ms
               // should be provided as argument to ensure immediate
               // trigger when the time is reached
  delay(1000);// do other things... like waiting one second between clock display

  if(digitalRead(2)){
    digitalWrite(5,HIGH);
  }else{
    digitalWrite(5,LOW);
  }

  leakDetect = analogRead(34);
  //Serial.print(leakDetect);
  if(leakDetect >= wetness){
    Serial.print("Lead Detect is: ");
    Serial.print(leakDetect);
    digitalWrite(15,HIGH);
  }else{
    digitalWrite(15,LOW);
  }
  
  if(!digitalRead(26)){
    //Serial.print("low sensor is in water"); 
    digitalWrite(32,HIGH);
  }else{
    digitalWrite(32,LOW);
    Serial.print("low sensor is in water"); 
  }
  
  if(awcEmpty){
    long awcEmptySec = doAwcEmpty();
    if(awcEmptySec != 0){
      Serial.print("Awc Empty seconds ran: ");
      Serial.println(awcEmptySec);
    }else if(awcEmptySec == 998){
      Serial.println("SUMP PUMP (EMPTY) RAN TOO LONG!");
    }
  }
  if(awcEmptyRan && !awcEmptyRunning){
    long awcFillSec = doAwcFill();
    if(awcFillSec !=0){
      Serial.print("Awc Fill seconds ran: ");
      Serial.println(awcFillSec);
    }else if(awcFillSec == 999){
      Serial.println("NEW PUMP (FILL) RAN TOO LONG!");
    }
  }

    if(ato && !awcEmpty && !awcEmptyRan){
    //Serial.println("About to execute ATO");
    long atoSec = doAto();
    if(atoSec != 0){
      Serial.print("Ato seconds ran: ");
      Serial.println(atoSec);
    }else if(atoSec == 997){
      Serial.println("RO PUMP RAN TOO LONG!");
    }
  }

/*  if(awcEmpty){
    Serial.println("About to execute AwcEmpty");
    doAwcEmpty{);
  }*/

  
}

// functions to be called when an alarm triggers:
/*void MorningAlarm() {
  Serial.println("Alarm: - turn lights off");
}

void EveningAlarm() {
  Serial.println("Alarm: - turn lights on");
}

void WeeklyAlarm() {
  Serial.println("Alarm: - its Saturday Morning");
}

void ExplicitAlarm() {
  Serial.println("Alarm: - this triggers only at the given date and time");
}

void Repeats() {
  Serial.println("15 second timer");
}

void Repeats2() {
  Serial.println("2 second timer");
}

void OnceOnly() {
  Serial.println("This timer only triggers once, stop the 2 second timer");
  // use Cron.free(id) to disable a timer and recycle its memory.
  Cron.free(id);
  // optional, but safest to "forget" the ID after memory recycled
  id = dtINVALID_ALARM_ID;
  // you can also use Cron.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}*/
  void TenMinute() {
  Serial.println("10 minute timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
  ato = true;
}
  void ThirtyMinute() {
  Serial.println("30 minute timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void OneHour() {
  Serial.println("1 hour timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void TwoHour() {
  Serial.println("2 hour timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void ThreeHour() {
  Serial.println("3 hour timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
  if(!atoRunning || !awcEmptyRunning || !awcFillRunning || !restart){
    ESP.restart();
    restart = true;
  }
}
  void FourHour() {
  Serial.println("4 hour timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
  restart = false;
}
  void EightHour() {
  Serial.println("8 hour timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void Midnight() {
  Serial.println("Midnight timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void OneAm() {
  Serial.println("1 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void TwoAm() {
  Serial.println("2 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void ThreeAm() {
  Serial.println("3 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void FourAm() {
  Serial.println("4 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void FiveAm() {
  Serial.println("5 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void SixAm() {
  Serial.println("6 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void SevenAm() {
  Serial.println("7 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void EightAm() {
  Serial.println("8 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void NineAm() {
  Serial.println("9 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void TenAm() {
  Serial.println("10 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
  awcEmpty = true;
}
  void ElevenAm() {
  Serial.println("11 am timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void Noon() {
  Serial.println("Noon timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void OnePm() {
  Serial.println("1 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void TwoPm() {
  Serial.println("2 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
  awcEmpty = true;
}
  void ThreePm() {
  Serial.println("3 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void FourPm() {
  Serial.println("4 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void FivePm() {
  Serial.println("5 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void SixPm() {
  Serial.println("6 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void SevenPm() {
  Serial.println("7 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void EightPm() {
  Serial.println("8 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void NinePm() {
  Serial.println("9 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void TenPm() {
  Serial.println("10 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}
  void ElevenPm() {
  Serial.println("11 pm timer");
  time_t tnow = time(nullptr);
  Serial.println(asctime(gmtime(&tnow)));
}

long doAto(){
  long retVal = 0;
  //Serial.println("In ATO");
  if(!atoRunning){
    Serial.println("setting atoRunning");
    beginAtoTime = millis();
    atoRunning = true;
  }
  if(digitalRead(2)){
    //Serial.println("Normal sensor out of water");
    //Serial.println("Turn on RO pump");
    long elaspe = millis() - beginAtoTime;
    atoRunTimeSec = elaspe/1000;
    if(atoRunTimeSec > atoLastAmount+(atoLastAmount*0.1)){  //ran too long
      digitalWrite(12,0);   //turn on ro pump 
        atoRunning = false;
        ato = false;
        return 997;    
    }
    digitalWrite(12,1);   //turn on ro pump
  }else{
    //Serial.println("Normal sensor in water");
    //Serial.println("Turn off RO pump");
    digitalWrite(12,0);  //turn off ro pump
      if(atoRunning){
        Serial.println("setting atoRunning FASLE");
        long elaspe = millis() - beginAtoTime;
        atoRunTimeSec = elaspe/1000;
        //Serial.print("Seconds ato ran: ");
        //Serial.println(atoRunTimeSec);
        atoRunning = false;
        ato = false;
        atoLastAmount = atoRunTimeSec;
        retVal = atoRunTimeSec;
      }
  }
  return retVal;
}

long doAwcEmpty(){
  long retVal = 0;
  //Serial.println("In ATO");
  if(!awcEmptyRunning){
    Serial.println("setting awcEmptyRunning");
    beginAwcEmptyTime = millis();
    awcEmptyRunning = true;
  }
    long elaspe = millis() - beginAwcEmptyTime;
    awcEmptyRunTimeSec = elaspe/1000;
    if(awcEmptyRunTimeSec > awcEmptyLastAmount+(awcEmptyRunTimeSec*0.1)){  //ran too long
      digitalWrite(14,0);   //turn on ro pump atoLastAmountawcEmptyLastAmount
      awcEmptyRunning = false;
      awcEmpty = false;
      awcEmptyRan = true;
      return 998;    
    }
  digitalWrite(14,HIGH); //turn sump pump on
  if(digitalRead(26)){
    Serial.print("low sensor out of water water");
    //Serial.println("Turn on RO pump");
    digitalWrite(32,1);
    digitalWrite(14,0);   //turn off sump pump
      if(awcEmptyRunning){
        Serial.println("setting awcEmptyRunning FASLE");
        long elaspe = millis() - beginAwcEmptyTime;
        awcEmptyRunTimeSec = elaspe/1000;
        //Serial.print("Seconds awcEmpty ran: ");
        //Serial.println(awcEmptyRunTimeSec);
        awcEmptyRunning = false;
        awcEmpty = false;
        awcEmptyRan = true;
        awcEmptyLastAmount = awcEmptyRunTimeSec;
        retVal = awcEmptyRunTimeSec;
      }
  }
  return retVal;
}

long doAwcFill(){
  long retVal = 0;
  //Serial.println("In ATO");
  if(!awcFillRunning){
    Serial.println("setting awcFillRunning");
    beginAwcFillTime = millis();
    awcFillRunning = true;
  }
  if(digitalRead(2)){
    //Serial.println("Normal sensor out of water");
    //Serial.println("Turn on RO pump");
    long elaspe = millis() - beginAwcFillTime;
    awcFillRunTimeSec = elaspe/1000;
    if(awcFillRunTimeSec > awcFillLastAmount+(awcFillLastAmount*0.1)){  //ran too long
      digitalWrite(27,0);   //turn on ro pump 
      awcFillRunning = false;
      awcFill = false;
      awcEmptyRan = false;
      return 999;    
    }
    digitalWrite(27,1);   //turn on new pump
  }else{
    //Serial.println("Normal sensor in water");
    //Serial.println("Turn off RO pump");
    digitalWrite(27,0);  //turn off new pump
      if(awcFillRunning){
        Serial.println("setting awcFillRunning FASLE");
        long elaspe = millis() - beginAwcFillTime;
        awcFillRunTimeSec = elaspe/1000;
        //Serial.print("Seconds ato ran: ");
        //Serial.println(atoRunTimeSec);
        awcFillRunning = false;
        awcFill = false;
        awcEmptyRan = false;
        awcFillLastAmount = awcFillRunTimeSec;
        retVal = awcFillRunTimeSec;
      }
  }
  return retVal;
}
