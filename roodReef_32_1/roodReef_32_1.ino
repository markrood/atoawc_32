#include <timer.h>


//#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager
#include <WiFi.h>
#include <Wire.h>
//#include <Hash.h>
//#include <WebServer.h>
//#include <WiFiManager.h> 
#include <FS.h>
#include <ArduinoJson.h>
#define WEBSERVER_H  // this has to be here and my libraries after this
#include "ESPAsyncWebServer.h"
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <NTPClient.h>
//#include <WiFiUdp.h>
#include <TimeLib.h>                 // Include Arduino time library
//#include <time.h>
//#include <ESP8266mDNS.h>
//#include "ESPFlash.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#define BUFFER_SIZE 5000
#include<vector>;
//main timer includes/////////
//#include "Adafruit_MCP23017.h"
#include <atoawc_util.h>
#include <timer.h>
#include <ato.h>
#include <awc.h>
#include <dosing.h>
#include <graphs.h>
#include <db.h>

unsigned long nowser = 0;
unsigned long awcRun = 0;
bool ftawc = true;
unsigned long nowsera = 0;
unsigned long atoRun = 0;
bool ftato = true;

///new lights sensors
//Adafruit_MCP23017 mcp;
bool calledBeginAto = false;
bool calledBeginAwc = false;
////////////////
String url = "";
String markKey1 = "fBplW8jJqqotTqTxck4oTdK_oHTJKAawKfja-WlcgW-";
const char* markKey = markKey1.c_str();
String ericKey1 = "bOZMgCFy7Bnhee7ZRzyi19";
const char* ericKey = ericKey1.c_str();

//WiFiManager wifiManager;
bool aNewDay = false;

//////////////////////////functions//////////////////////////
void notFound(AsyncWebServerRequest *request);
String processor(const String& var);
void startWifi();
void startSpiffs();
void printWebValue();
float secondsToGallons(int seconds);
int secondsToMilliliters(int seconds);

////////////////json month/////////////
StaticJsonDocument<1024> docOut;

////////////////////////////////////
String iPAddress;
const char* host = "maker.ifttt.com";
const int httpsPort = 80;
//WiFiServer server(httpsPort);
AsyncWebServer server(80);
DNSServer dns;
  bool firstTime = false;

  int sentWLCount;
  int sentWHCount;
  int sentLeakCount;
WiFiClient client;
int sendHttp(String message);
String jsonAto;
String jsonYearAto;
String jsonYearAwc;
String jsonYearBlue;
String jsonYearGreen;
String jsonYearYellow;

 const int resPin = A0;

String buildYearJson(String type,String testMonth);

void ICACHE_RAM_ATTR ISRoutine ();


// Variable to store the HTTP request
String header;

const char* PARAM_IFTTT = "ifttt";
const char* PARAM_AWCFREQ = "awcfreq";
const char* PARAM_ATOFREQ = "atofreq";
const char* PARAM_ATOTOD  = "atotod";
const char* PARAM_LEAKNOTFREQ = "leaknotfreq";
const char* PARAM_LWNOTFREQ = "lwnotfreq";
const char* PARAM_HWNOTFREQ = "hwnotfreq";
const char* PARAM_HBNOTFREQ = "hbnotfreq";
const char* PARAM_BDNOTFREQ = "bdnotfreq";
const char* PARAM_GDNOTFREQ = "gdnotfreq";
const char* PARAM_YDNOTFREQ = "ydnotfreq";
const char* PARAM_LEAKLEVEL = "leaklevel";
const char* PARAM_ATO = "ato";
const char* PARAM_TZ = "timezone";

//const char* PARAM_DIAL = "example";
const char* PARAM_INPUT1 = "value1";
const char* PARAM_INPUT2 = "value2";
const char* PARAM_INPUT3 = "value3";
const char* PARAM_CHKBOXINPUT1 = "value";
const char* PARAM_CHKBOXINPUT2 = "value";
const char* PARAM_CHKBOXINPUT3 = "value";;

long utcOffsetInSeconds = 0;
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String slider1;
String slider2;
String slider3;
String chkboxValue1;
String chkboxValue2;
String chkboxValue3;
bool ckBox1 = false;
bool ckBox2 = false;
bool ckBox3 = false;

//doser client stuff
int sensorValue0 = 0;       //sensor value, I'm usingg 0/1 button state
int sensorValue1 = 0;        
int sensorValue2 = 0;        
int sensorValue3 = 0;  

void sendJson();

bool calibrating = false;
int oneHundredmlBlueTime = 0;
int oneHundredmlGreenTime = 0;
int oneHundredmlYellowTime = 0;
bool blueStarted = false;
bool greenStarted = false;
bool yellowStarted = false;
int webSerialCounter1 = 0;
int webSerialCounter2 = 0;
 unsigned long atoStart= 0, atoStop= 0, awcStart=0,awcStop=0, yellowStart=0, yellowStop=0;
 unsigned long  greenStart=0, greenStop=0, blueStart=0,blueStop=0;

 bool executingAto = false;
 bool executingAwc = false;
 bool executingBlue = false;
 bool executingGreen = false;
 bool executingYellow = false;
 int resVal;
 bool functionAwc = false;
unsigned long startedTime = 0;
 int aveCount = 0;

 int testTimer = 0;
 bool testTimerRunning = false;
 int currentTestTimer = 0;
 bool testAwcExecuted = false;


// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Roodreef</title>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js"></script>
  <script src="https://use.fontawesome.com/releases/v5.0.8/js/all.js"></script>
  <link href="style.css" rel="stylesheet">
  <link rel="stylesheet" href="https://m.w3newbie.com/you-tube.css">
<script>
    function submitMessage() {
      alert("Save value");
      setTimeout(function(){ document.location.reload(false); }, 500);
      window.location.reload();   
    }
</script>
  </head>
<body>
<!-- Navigation -->
<nav class="navbar-expand-md navbar-light bg-light sticy-top">
  <div class="container-fluid">
    <a class="navbar-brand" href="#"><img src ="/morish.jpg"><a/>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive">
      <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarResponsive" >
      <ul class="navbar-nav ml-auto">
        <li class="nav-item active">
          <a class="nav-link" href="/">Home</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="/chart">Chart</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="/doser">Dosing</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="/manual">Manual</a>
        </li>
      </ul>
    </div>
  </div>
  </nav>
<form id="AWC" action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">ifttt (current value <b>%ifttt%</b>):<label for="ifttt">IFTTT Key</label></p>
     </div>
     <div>
      <input type="text" name="ifttt" id="ifttt" class="form-control">
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="ifttt" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
    </form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Auto Water Change (AWC) Frequency (current value <b>%awcfreq%</b>):<label for="awcfreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="awcfreq" name="awcfreq">
      <option selected>Choose Auto Water Change (AWC) Frequency....</option>
      <option value ="twiceaday">Twice a Day</option>
      <option value ="onceaday">Once a Day</option>
      <option value="everyother">Every Other Day</option>
      <option value="onceaweek">Once a Week</option>
      <option value="onceamonth">Once a Month</option>
      <option value="never">Never</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="awcfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Auto Top Off (ATO) Frequency (current value <b>%atofreq%</b>):<label for="atofreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="atofreq" name="atofreq">
      <option selected>Choose Auto Top Off (AWC Frequency)....</option>
      <option value ="always">Always</option>
      <option value ="1hour">One Hour</option>
      <option value ="2hour">Two Hour</option>
      <option value="3hour">Three Hour</option>
      <option value="4hour">Four Hour</option>
      <option value="onceaday">Once a Day</option>
      <option value="always">Always</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="awcfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Auto Water Change (AWC) Time Of Day (TOD) Frequency (current value <b>%atotod%</b>):<label for="atotod">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="atotod" name="atotod">
      <option selected>Choose ATO TOD Frequency....</option>
      <option value ="1">One oclock</option>
      <option value ="2">Two oclock</option>
      <option value="3">Three oclock</option>
      <option value="4">Four oclock</option>
      <option value="5">Five oclcok</option>
      <option value="6">Six oclock</option>
      <option value ="7">Seven oclock</option>
      <option value ="8">Eight oclock</option>
      <option value="9">Nine oclock</option>
      <option value="10">Ten oclock</option>
      <option value="11">Eleven oclcok</option>
      <option value="12">Twelve oclock</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="awcfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Leak Notification Frequency (current value <b>%leaknotfreq%</b>):<label for="leaknotfreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="leaknotfreq" name="leaknotfreq">
      <option selected>Choose Leak Notification Frequency....</option>
      <option value ="halfhour">Every Half Hour</option>
      <option value ="onehour">Once an Hour</option>
      <option value="4hour">Every 4 Hours</option>
      <option value="8hour">Every 8 Hours</option>
      <option value="onceaday">Once a Day</option>
      <option value="never">Never</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="leaknotfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">High Water Notification Frequency (current value <b>%hwnotfreq%</b>):<label for="hwnotfreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="hwnotfreq" name="hwnotfreq">
      <option selected>Choose High Water Notification Frequency....</option>
      <option value ="halfhour">Every Half Hour</option>
      <option value ="onehour">Once an Hour</option>
      <option value="4hour">Every 4 Hours</option>
      <option value="8hour">Every 8 Hours</option>
      <option value="onceaday">Once a Day</option>
      <option value="never">Never</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="hwnotfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Low Water Notification Frequency (current value <b>%lwnotfreq%</b>):<label for="hwnotfreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="lwnotfreq" name="lwnotfreq">
      <option selected>Choose Low Water Notification Frequency....</option>
      <option value ="halfhour">Every Half Hour</option>
      <option value ="onehour">Once an Hour</option>
      <option value="4hour">Every 4 Hours</option>
      <option value="8hour">Every 8 Hours</option>
      <option value="onceaday">Once a Day</option>
      <option value="never">Never</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="lwnotfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Heartbeat Notification Frequency (current value <b>%hbnotfreq%</b>):<label for="hbnotfreq">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="hbnotfreq" name="hbnotfreq">
      <option selected>Choose Heartbeat Notification Frequency....</option>
      <option value ="halfhour">Every Half Hour</option>
      <option value ="onehour">Once an Hour</option>
      <option value="4hour">Every 4 Hours</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="hbnotfreq" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Leak Detection Level (current value <b>%leaklevel%</b>):<label for="leaklevel">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="leaklevel" name="leaklevel">
      <option selected>Choose Leak Detection Level....</option>
      <option value ="moister">Moisture</option>
      <option value ="damp">Damp</option>
      <option value="wet">Wet</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="leaklevel" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">ATO Notification (current value <b>%ato%</b>):<label for="ato">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="ato" name="ato">
      <option selected>Choose ATO Message....</option>
      <option value ="on">On</option>
      <option value ="off">Off</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="ato" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<form action="/get" target="hidden-form" style="width: 400px; margin: auto">
    <div class="input-group col-md-30">
     <p style="color:blue" style="font-size:60px">Timezone (current value <b>%timezone%</b>):<label for="timezone">:</label></p>
     </div>
      <div>
       <select class="custom-select" id="timezone" name="timezone">
   <option></option>
   <option value="Etc/GMT+12">(GMT-12:00) International Date Line West</option>
   <option value="Pacific/Midway">(GMT-11:00) Midway Island, Samoa</option>
   <option value="Pacific/Honolulu">(GMT-10:00) Hawaii</option>
   <option value="US/Alaska">(GMT-09:00) Alaska</option>
   <option value="America/Los_Angeles">(GMT-08:00) Pacific Time (US & Canada)</option>
   <option value="America/Tijuana">(GMT-08:00) Tijuana, Baja California</option>
   <option value="US/Arizona">(GMT-07:00) Arizona</option>
   <option value="America/Chihuahua">(GMT-07:00) Chihuahua, La Paz, Mazatlan</option>
   <option value="US/Mountain">(GMT-07:00) Mountain Time (US & Canada)</option>
   <option value="America/Managua">(GMT-06:00) Central America</option>
   <option value="US/Central">(GMT-06:00) Central Time (US & Canada)</option>
   <option value="America/Mexico_City">(GMT-06:00) Guadalajara, Mexico City, Monterrey</option>
   <option value="Canada/Saskatchewan">(GMT-06:00) Saskatchewan</option>
   <option value="America/Bogota">(GMT-05:00) Bogota, Lima, Quito, Rio Branco</option>
   <option value="US/Eastern">(GMT-05:00) Eastern Time (US & Canada)</option>
   <option value="US/East-Indiana">(GMT-05:00) Indiana (East)</option>
   <option value="Canada/Atlantic">(GMT-04:00) Atlantic Time (Canada)</option>
   <option value="America/Caracas">(GMT-04:00) Caracas, La Paz</option>
   <option value="America/Manaus">(GMT-04:00) Manaus</option>
   <option value="America/Santiago">(GMT-04:00) Santiago</option>
   <option value="Canada/Newfoundland">(GMT-03:30) Newfoundland</option>
   <option value="America/Sao_Paulo">(GMT-03:00) Brasilia</option>
   <option value="America/Argentina/Buenos_Aires">(GMT-03:00) Buenos Aires, Georgetown</option>
   <option value="America/Godthab">(GMT-03:00) Greenland</option>
   <option value="America/Montevideo">(GMT-03:00) Montevideo</option>
   <option value="America/Noronha">(GMT-02:00) Mid-Atlantic</option>
   <option value="Atlantic/Cape_Verde">(GMT-01:00) Cape Verde Is.</option>
   <option value="Atlantic/Azores">(GMT-01:00) Azores</option>
   <option value="Africa/Casablanca">(GMT+00:00) Casablanca, Monrovia, Reykjavik</option>
   <option value="Etc/Greenwich">(GMT+00:00) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London</option>
   <option value="Europe/Amsterdam">(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna</option>
   <option value="Europe/Belgrade">(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague</option>
   <option value="Europe/Brussels">(GMT+01:00) Brussels, Copenhagen, Madrid, Paris</option>
   <option value="Europe/Sarajevo">(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb</option>
   <option value="Africa/Lagos">(GMT+01:00) West Central Africa</option>
   <option value="Asia/Amman">(GMT+02:00) Amman</option>
   <option value="Europe/Athens">(GMT+02:00) Athens, Bucharest, Istanbul</option>
   <option value="Asia/Beirut">(GMT+02:00) Beirut</option>
   <option value="Africa/Cairo">(GMT+02:00) Cairo</option>
   <option value="Africa/Harare">(GMT+02:00) Harare, Pretoria</option>
   <option value="Europe/Helsinki">(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius</option>
   <option value="Asia/Jerusalem">(GMT+02:00) Jerusalem</option>
   <option value="Europe/Minsk">(GMT+02:00) Minsk</option>
   <option value="Africa/Windhoek">(GMT+02:00) Windhoek</option>
   <option value="Asia/Kuwait">(GMT+03:00) Kuwait, Riyadh, Baghdad</option>
   <option value="Europe/Moscow">(GMT+03:00) Moscow, St. Petersburg, Volgograd</option>
   <option value="Africa/Nairobi">(GMT+03:00) Nairobi</option>
   <option value="Asia/Tbilisi">(GMT+03:00) Tbilisi</option>
   <option value="Asia/Tehran">(GMT+03:30) Tehran</option>
   <option value="Asia/Muscat">(GMT+04:00) Abu Dhabi, Muscat</option>
   <option value="Asia/Baku">(GMT+04:00) Baku</option>
   <option value="Asia/Yerevan">(GMT+04:00) Yerevan</option>
   <option value="Asia/Kabul">(GMT+04:30) Kabul</option>
   <option value="Asia/Yekaterinburg">(GMT+05:00) Yekaterinburg</option>
   <option value="Asia/Karachi">(GMT+05:00) Islamabad, Karachi, Tashkent</option>
   <option value="Asia/Calcutta">(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi</option>
   <option value="Asia/Calcutta">(GMT+05:30) Sri Jayawardenapura</option>
   <option value="Asia/Katmandu">(GMT+05:45) Kathmandu</option>
   <option value="Asia/Almaty">(GMT+06:00) Almaty, Novosibirsk</option>
   <option value="Asia/Dhaka">(GMT+06:00) Astana, Dhaka</option>
   <option value="Asia/Rangoon">(GMT+06:30) Yangon (Rangoon)</option>
   <option value="Asia/Bangkok">(GMT+07:00) Bangkok, Hanoi, Jakarta</option>
   <option value="Asia/Krasnoyarsk">(GMT+07:00) Krasnoyarsk</option>
   <option value="Asia/Hong_Kong">(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi</option>
   <option value="Asia/Kuala_Lumpur">(GMT+08:00) Kuala Lumpur, Singapore</option>
   <option value="Asia/Irkutsk">(GMT+08:00) Irkutsk, Ulaan Bataar</option>
   <option value="Australia/Perth">(GMT+08:00) Perth</option>
   <option value="Asia/Taipei">(GMT+08:00) Taipei</option>
   <option value="Asia/Tokyo">(GMT+09:00) Osaka, Sapporo, Tokyo</option>
   <option value="Asia/Seoul">(GMT+09:00) Seoul</option>
   <option value="Asia/Yakutsk">(GMT+09:00) Yakutsk</option>
   <option value="Australia/Adelaide">(GMT+09:30) Adelaide</option>
   <option value="Australia/Darwin">(GMT+09:30) Darwin</option>
   <option value="Australia/Brisbane">(GMT+10:00) Brisbane</option>
   <option value="Australia/Canberra">(GMT+10:00) Canberra, Melbourne, Sydney</option>
   <option value="Australia/Hobart">(GMT+10:00) Hobart</option>
   <option value="Pacific/Guam">(GMT+10:00) Guam, Port Moresby</option>
   <option value="Asia/Vladivostok">(GMT+10:00) Vladivostok</option>
   <option value="Asia/Magadan">(GMT+11:00) Magadan, Solomon Is., New Caledonia</option>
   <option value="Pacific/Auckland">(GMT+12:00) Auckland, Wellington</option>
   <option value="Pacific/Fiji">(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option>
   <option value="Pacific/Tongatapu">(GMT+13:00) Nuku'alofa</option>
      </select>
      <div class="input-group-append">
        <button class="btn btn-primary" type="submit" id="timezone" value = "Submit" onclick="submitMessage()">Save</button>
      </div>
    </div>
</form><br>
<iframe style="display:none" name="hidden-form" ></iframe>
</body>
</html>)rawliteral";



///////////////////////////////////////////////////////////////////////
// this is where the timers get added
//////////////////////////////////////////////////////////////////////
int spiffsLReset;
int spiffsHWReset;
int spiffsLWReset;
int spiffsAtoReset;
int spiffsAwcReset;
int spiffsHbReset;
int spiffsBdReset;
int spiffsGdReset;
int spiffsYdReset;
int currentTimeArr[5];

bool timezoneSet = false;


bool sentAtoHttp = false;
bool sentAwcHttp = false;
bool sentBlueHttp = false;
bool sentGreenHttp = false;
bool sentYellowHttp = false;

bool atoRunning = false;
bool awcRunning = false; //TODO use this


////////////////////Pointer stuff///////////////////
AtoAwcUtil *atoAwcUtil;
Timer *timer;
Ato *ato;
Awc *awc;
Graphs *graphs;
Dosing *dosing;
Db *db;


/*void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}*/

/////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////
 //  setup
///////////////////////////////////////////////////////////////////////////
  void setup() {

    startSpiffs();
    startWifi();
    atoAwcUtil = new AtoAwcUtil(&server);
    atoAwcUtil->beginUtil();// have to start WebSerial DON'T FORGET THIS
    timer = new Timer(atoAwcUtil);
    ato = new Ato(atoAwcUtil);
    awc = new Awc(atoAwcUtil);
    graphs = new Graphs(atoAwcUtil);
    dosing = new Dosing(atoAwcUtil, client);
    db = new Db();

  //mcp.begin();
  /*mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, INPUT);
  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, LOW);
  mcp.digitalWrite(2, LOW);
  mcp.pinMode(6, OUTPUT);
   mcp.pinMode(3, INPUT);
  mcp.pinMode(4, INPUT);
  mcp.pinMode(5, INPUT);
  mcp.pinMode(7, INPUT);
  mcp.pullUp(7, HIGH);
    mcp.pinMode(8,OUTPUT);
  mcp.digitalWrite(8,LOW);
  mcp.pinMode(9,OUTPUT);
  mcp.digitalWrite(9,LOW);
  mcp.pinMode(10,OUTPUT);
  mcp.digitalWrite(10,LOW);


  //mcp.begin();*/
    Serial.begin(115200);
  AsyncWiFiManager wifiManager(&server,&dns);
/*  IPAddress _ip = IPAddress(192, 168, 1, 78);
  IPAddress _gw = IPAddress(192, 168, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);*/
  IPAddress _ip = IPAddress(172, 20, 10, 78);
  IPAddress _gw = IPAddress(172, 20, 10, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 240);

  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
    delay(3000);
    // ESP.reset();
    delay(5000);
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  ////////////key work///////////////
  String ift = atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");
  //Serial.print("ift key is ");
  //Serial.println(ift);
  if(ift.length() <=5){
    atoAwcUtil->writeFile(SPIFFS, "/ifttt.txt",markKey);
    //Serial.println("In write ifttt key");
  }
  //////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//
//  this just prints out the content of spiffs
//
//////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////CREATE TEST ARRAY///////////////////////////////////
//////////////////this is for testing  we are currently using this arr in chart
//TODO  remove this test array
/*if(TEST_MODE){
  for(int x=1;x<13;x++){
    if(x==1 || x== 3 || x==5 || x==7 || x==8 || x==10 || x==12){
      for(int i=1;i<32;i++){
        writeArray("ato",2020,changeMonthNubToStr(x), i,float(random(1,80)));
        writeArray("awc",2020,changeMonthNubToStr(x), i,float(random(1,100)));
        writeArray("blue",2020,changeMonthNubToStr(x), i,float(random(2,50)));
        writeArray("green",2020,changeMonthNubToStr(x), i,float(random(2,40)));
        writeArray("yellow",2020,changeMonthNubToStr(x), i,float(random(2,45)));
        //delay(500);
      }
    }else if(x==4 || x==6 || x==9 || x==11){
       for(int i=1;i<31;i++){
        writeArray("ato",2020,changeMonthNubToStr(x), i,float(random(1,80)));
        writeArray("awc",2020,changeMonthNubToStr(x), i,float(random(1,100)));
        writeArray("blue",2020,changeMonthNubToStr(x), i,float(random(9,11)));
        writeArray("green",2020,changeMonthNubToStr(x), i,float(random(9,11)));
        writeArray("yellow",2020,changeMonthNubToStr(x), i,float(random(9,11)));
         //delay(500);
      }   
    }else{
       for(int i=1;i<28;i++){
        writeArray("ato",2020,changeMonthNubToStr(x), i,float(random(1,75)));
        writeArray("awc",2020,changeMonthNubToStr(x), i,float(random(1,125)));
        writeArray("blue",2020,changeMonthNubToStr(x), i,float(random(2,60)));
        writeArray("green",2020,changeMonthNubToStr(x), i,float(random(5,70)));
        writeArray("yellow",2020,changeMonthNubToStr(x), i,float(random(8,80)));
         //delay(500);
      }   
    }
  }
}*/
///////////////////////////////READ TEST ARRAY //////////////////////////////
//TODO remove this test array
  //jsonAto = readFile(SPIFFS, myStr.c_str());
  //jsonAto = readFile(SPIFFS,  "/octoberatoArr.txt");
  //Serial.println("printing jsonAto file from Spiifs");
  //Serial.print("Printing jsonATo read from spiffs "+jsonAto);

  //TODO need to retrieve json for year month
  //String prefix = "json";
 
//Serial.println("ato zero vector is "+atoYearV[0]);
//Serial.println("yellow last vector is "+yellowYearV[11]);
//Serial.println("blue last vector is "+blueYearV[11]);
//Serial.println("The year array for ato is "+buildYearJson("ato"));
//jsonAto = atoYearV[3];
//jsonAto = buildYearJson("ato");
//Serial.println("jsonAto is "+jsonAto);
///////////////////////////////////////////////////////////////////////////////////

    ///////////////Web Server Setup /////////////////
    Serial.begin(115200);


    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(WiFi.SSID());
    //  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      //atoAwcUtil->webSerialWrite(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    iPAddress = WiFi.localIP().toString();
    Serial.println(WiFi.localIP());

    server.begin();


    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/html", index_html, processor);
    });

   // Send a GET request to <ESP_IP>/get?ifttt=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
      String inputMessage;
      // GET ifttt value on <ESP_IP>/get?ifttt=<inputMessage>
      if (request->hasParam(PARAM_IFTTT)) {
        inputMessage = request->getParam(PARAM_IFTTT)->value();
        atoAwcUtil->writeFile(SPIFFS, "/ifttt.txt", inputMessage.c_str());
      }

      // GET awcfreq value on <ESP_IP>/get?awcfreq=<inputMessage>
      if (request->hasParam(PARAM_AWCFREQ)) {
        inputMessage = request->getParam(PARAM_AWCFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/awcfreq.txt", inputMessage.c_str());
      }
      // GET atofreq value on <ESP_IP>/get?awcfreq=<inputMessage>
      if (request->hasParam(PARAM_ATOFREQ)) {
        inputMessage = request->getParam(PARAM_ATOFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/atofreq.txt", inputMessage.c_str());
      }
      // GET awcfreq value on <ESP_IP>/get?awcfreq=<inputMessage>
      if (request->hasParam(PARAM_ATOTOD)) {
        inputMessage = request->getParam(PARAM_ATOTOD)->value();
        atoAwcUtil->writeFile(SPIFFS, "/atotod.txt", inputMessage.c_str());
      }
      // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
      else if (request->hasParam(PARAM_LEAKNOTFREQ)) {
        inputMessage = request->getParam(PARAM_LEAKNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/leaknotfreq.txt", inputMessage.c_str());
      }
      // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(PARAM_LWNOTFREQ)) {
        inputMessage = request->getParam(PARAM_LWNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/lwnotfreq.txt", inputMessage.c_str());
      }
      // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(PARAM_HWNOTFREQ)) {
        inputMessage = request->getParam(PARAM_HWNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/hwnotfreq.txt", inputMessage.c_str());
      }
       // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(PARAM_HBNOTFREQ)) {
        inputMessage = request->getParam(PARAM_HBNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/hbnotfreq.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_LEAKLEVEL)) {
        inputMessage = request->getParam(PARAM_LEAKLEVEL)->value();
        atoAwcUtil->writeFile(SPIFFS, "/leaklevel.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_ATO)) {
        inputMessage = request->getParam(PARAM_ATO)->value();
        atoAwcUtil->writeFile(SPIFFS, "/ato.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_TZ)) {
        inputMessage = request->getParam(PARAM_TZ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/timezone.txt", inputMessage.c_str());
        Serial.println("just wrote timezone :"+inputMessage);
      }
      else if (request->hasParam(PARAM_BDNOTFREQ)) {
        inputMessage = request->getParam(PARAM_BDNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/bdnotfreq.txt", inputMessage.c_str());
        Serial.println("wrote bdnotfreq txt");
      }
      else if (request->hasParam(PARAM_GDNOTFREQ)) {
        inputMessage = request->getParam(PARAM_GDNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/gdnotfreq.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_YDNOTFREQ)) {
        inputMessage = request->getParam(PARAM_YDNOTFREQ)->value();
        atoAwcUtil->writeFile(SPIFFS, "/ydnotfreq.txt", inputMessage.c_str());
      }
/*      else if (request->hasParam(PARAM_DIAL)) {
        inputMessage = request->getParam(PARAM_DIAL)->value();
        atoAwcUtil->writeFile(SPIFFS, "/dial.txt", inputMessage.c_str());
        Serial.println("just wrote dial :"+inputMessage);
      }*/
     else {
        inputMessage = "No message sent";
      }
      //Serial.println(inputMessage);
      request->send(200, "text/text", inputMessage);
    });
    server.on("/jsonAto", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",  jsonYearAto);
  }); 
    server.on("/jsonAwc", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",  jsonYearAwc);
  }); 
    server.on("/jsonBlue", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",  jsonYearBlue);
  }); 
    server.on("/jsonGreen", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",  jsonYearGreen);
  }); 
    server.on("/jsonYellow", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json",  jsonYearYellow);
  }); 
 
  server.on("/morish.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/morish.jpg", "image/jpg");
  });

  // Route for root / web page
  server.on("/chart", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/charts.html");
  });

  // Route for root / web page
  server.on("/doser", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dial.html", String(),false,processor);
  });

  server.on("/package", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/package.json");
  });

   server.on("/pureknob.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/pureknob.js");
  });

    server.onNotFound(notFound);
    server.begin();  /////////probably have to change THIS

//////////////////////Doser//////////////////////////////////////////
      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT1)) {
      inputMessage = request->getParam(PARAM_INPUT1)->value();
      slider1 = inputMessage;
      //atoAwcUtil->writeFile(SPIFFS, "/awcfreq.txt", inputMessage.c_str());
      atoAwcUtil->writeFile(SPIFFS, "/slider1.txt", slider1.c_str());
      //Serial.println("slider value 1 izzzzzzzzzzzzzzzzzzzzzs "+slider1);
      atoAwcUtil->webSerialWrite("Blue slider value is "+slider1);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT2)) {
      inputMessage = request->getParam(PARAM_INPUT2)->value();
      slider2 = inputMessage;
      atoAwcUtil->writeFile(SPIFFS, "/slider2.txt", slider2.c_str());
      Serial.println("slider value 2 is "+slider2);
      atoAwcUtil->webSerialWrite("Green slider value is "+slider2);
      
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT3)) {
      inputMessage = request->getParam(PARAM_INPUT3)->value();
      slider3 = inputMessage;
      atoAwcUtil->writeFile(SPIFFS, "/slider3.txt", slider3.c_str());
      Serial.println("slider value 3 is "+slider3);
      atoAwcUtil->webSerialWrite("Yellow slider value is "+slider3);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
       // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT1)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT1)->value();
      chkboxValue1 = inputMessage;
      if(chkboxValue1 == "true"){
        ckBox1 = true;
      }else if(chkboxValue1 = "false"){
        ckBox1 = false;
      }
      Serial.println("checkbox value 1 is "+chkboxValue1);
      atoAwcUtil->webSerialWrite("Blue checkbox value is "+chkboxValue1);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT2)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT2)->value();
      chkboxValue2 = inputMessage;
      if(chkboxValue2 == "true"){
        ckBox2 = true;
      }else if(chkboxValue2 = "false"){
        ckBox2 = false;
      }
      Serial.println("checkbox value 2 is "+chkboxValue2);
      atoAwcUtil->webSerialWrite("Green checkbox value is "+chkboxValue2);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
      // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT3)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT3)->value();
      chkboxValue3 = inputMessage;
      if(chkboxValue3 == "true"){
        ckBox3 = true;
      }else if(chkboxValue3 = "false"){
        ckBox3 = false;
      }
      Serial.println("checkbox value 3 is "+chkboxValue3);
      atoAwcUtil->webSerialWrite("Yellow checkbox value is "+chkboxValue3);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  }); 



  //**************timer stuff added to setup ******************/////////////////
  spiffsLReset = 1; // TODO this is used to reset notification or first time through
  spiffsHWReset = 1; //TODO this is used to reset notification or first time through
  spiffsLWReset = 1; //TODO this is used to reset notification or first time through
  spiffsAtoReset = 1; //TODO this is used to reset notification or first time through
  spiffsAwcReset = 1; //TODO this is used to reset notification or first time through
  spiffsHbReset = 1; //TODO this is used to reset notification or first time through
  spiffsBdReset = 1; //TODO this is used to reset notification or first time through
  spiffsGdReset = 1; //TODO this is used to reset notification or first time through
  spiffsYdReset = 1; //TODO this is used to reset notification or first time through

   //WebSerial.begin(&server);
   //WebSerial.msgCallback(recvMsg);
 sendHttp("Email");//TODO what is this
//send_webhook("email","fBplW8jJqqotTqTxck4oTdK_oHTJKAawKfja-WlcgW-","Hi its Wemos!","value 2","value 3");
  } ///////////////////////end of setup ////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////

void loop() {
  int xx = 0;
  String changing = String(xx);
  db->putString("/atoawc",changing);//+"{skdjfksdfajk, skdfjaksdjf, sdkfjalsjdf[0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000]");
  //calibration has top priority
  if(ckBox1 || ckBox2  || ckBox3 || calibrating){

      calibrating = true;
      dosing->setCalibrating(true);
      dosing->setCkBox1(ckBox1);
      dosing->setCkBox2(ckBox2);
      dosing->setCkBox3(ckBox3);
      dosing->calibrate();
      calibrating = dosing->getCalibrating();//TODO need to test this

  }else{
    calibrating = false;
    if (WiFi.status() != WL_CONNECTED)
    {
      startWifi();
    }
    
   // MDNS.update();
    //delay(1000);
  //timeClient.update();
  //WebSerial.print("hs 192.168.4.1/, ato_awc_");
  String ipa = WiFi.localIP().toString();
  //WebSerial.print(ipa+",");
  //WebSerial.println("!");
    //delay(1000);
 //Serial.println(s3);
  // put your main code here, to run repeatedly:
/*  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Reseting");  //TODO do we need this?
    WebSerial.println("Reseting");
    ESP.restart();
  }else{
    //Serial.print(".");
  }*/
  if(firstTime){
    //This just send text ESP is running
    //sendHttp("ESP");
    sendHttp("ATO_AWC_Started");

      //Serial.println("trying to send ESP");
//      int status = ("ESP");
      firstTime = false;
      timer->setYrMoDay();
  }
///////////////////////////////main timers looping/////////////////
  if(timezoneSet){// don't do anything until timezone is set
      /////// time stuff //////////////////////////////////////////////
  ////////**** use this timezone stuff**************////////////////
  // Init and get the time
  //timeClient.begin();
    atoAwcUtil->setCurrentTime(); //TODO store current day in SPIFFS
    timer->setYrMoDay();// this is used to see if day changed from now
    timer->setCurrentDay();

      //timer->setCurrentTime();
     bool fuctionNewDay = timer->execute("dayChange");
     //fuctionNewDay = true;
     if(fuctionNewDay){
      Serial.println("executing new day");
      atoAwcUtil->webSerialWrite("executing new day");
      String daileyatoFill = atoAwcUtil->readFile(SPIFFS, "/atodailyfill.txt");
      int atoDFill = daileyatoFill.toInt();
      float valato = secondsToGallons(atoDFill);
      Serial.println("ato daily gallon is "+String(valato));
      String daileyStr = "0";
      atoAwcUtil->writeFile(SPIFFS, "/atodailyfill.txt", daileyStr.c_str());
      graphs->closeDayPutInArr("ato",valato,0,0,0);//the 0,0 is for test mode
      String daileyawcFill = atoAwcUtil->readFile(SPIFFS, "/awcdailyfill.txt");
      int awcDFill = daileyawcFill.toInt();
      float valawc = secondsToGallons(awcDFill);
      Serial.println("awc daily gallon is "+String(valawc));
      daileyStr = "0";
      atoAwcUtil->writeFile(SPIFFS, "/awcdailyfill.txt", daileyStr.c_str());
      graphs->closeDayPutInArr("awc",valawc,0,0,0);
      //NOTE dosing already in ml!!!!
      String daileybdFill = atoAwcUtil->readFile(SPIFFS, "/bddailyfill.txt");
      float bdDFill = daileybdFill.toInt();
      Serial.println("blue dose daily ml is "+String(bdDFill));
      daileyStr = "0";
      atoAwcUtil->writeFile(SPIFFS, "/bddailyfill.txt", daileyStr.c_str());
      graphs->closeDayPutInArr("blue",bdDFill,0,0,0);
      String daileygdFill = atoAwcUtil->readFile(SPIFFS, "/gddailyfill.txt");
      float gdDFill = daileygdFill.toInt();
      Serial.println("green dose daily ml is "+String(gdDFill));
      daileyStr = "0";
      atoAwcUtil->writeFile(SPIFFS, "/gddailyfill.txt", daileyStr.c_str());
      graphs->closeDayPutInArr("green",gdDFill,0,0,0);
      String daileyydFill = atoAwcUtil->readFile(SPIFFS, "/yddailyfill.txt");
      float ydDFill = daileyydFill.toInt();
      Serial.println("yellow dose daily ml is "+String(ydDFill));
      daileyStr = "0";
      atoAwcUtil->writeFile(SPIFFS, "/yddailyfill.txt", daileyStr.c_str());
      graphs->closeDayPutInArr("yellow",ydDFill,0,0,0);
      //timer->setYrMoDay();//do this in timer now
      //yrMoDay[0] = currentTimeArr[0];  //FINALLY CHANGE TO THE NEW DAY After writing array
      //yrMoDay[1] = currentTimeArr[1];
      //yrMoDay[2] = currentTimeArr[2];
      Serial.println("finished newDay");
     }
//////////new with lights for sensor ////////////////////

  ato->doWaterLights();
  
  Serial.println("after doWaterLIghts()");


////////////////////////////////////////////////////////
     bool fuctionLeak = timer->execute("leakNot");
     if(fuctionLeak){
       Serial.println("executing leak");
       atoAwcUtil->webSerialWrite("EL_1,");
          //sendHttp("Leak");
        if(ato->checkLeaks()){
         sendHttp("Leak");
         atoAwcUtil->webSerialWrite("EL_1_A_Leak_$, ");
        }
     }
     Serial.println("after leakNot");
     //delay(1000);
     bool fuctionHW = timer->execute("highWaterNot");
       if(fuctionHW){
        Serial.println("executing high water");
        atoAwcUtil->webSerialWrite("EHW_2,");
          //sendHttp("HighWater");
        if(ato->checkHighWater()){
          atoAwcUtil->webSerialWrite("EHW_2_A_High water_!, ");
         sendHttp("HighWater");
        }
     }
     Serial.println("after highWaterNot");
     //delay(10000);
     bool fuctionLW = timer->execute("lowWaterNot");
     if(fuctionLW){
        Serial.println("executing low water");
        atoAwcUtil->webSerialWrite("ELW_3,");
          //sendHttp("LowWater");
        if(ato->checkLowWater()){
          sendHttp("LowWater,");
          atoAwcUtil->webSerialWrite("ELW_3_A_Low water_!, ");
        }
     }
     Serial.println("after lowWaterNot");
     //delay(1000);
     ato->setWaterChange(false);
     /////////////////////    timer  ///////////////////////////////
     
      functionAwc = timer->execute("awcNot");
      //bool functionAwc = timer->executeTest("skippy", 900000);
      //bool functionAwc = false;
     if(functionAwc){
      executingAwc = true;
      awc->setExecutingAwc(true);
      awc->setWaterChange(true);
      awc->setWaterEmptied(false);
      functionAwc = false;
      if(!awc->getSentAwcHttp()){
        sendHttp("Auto_Water_Change");
        sentAwcHttp = true;
        awc->setSentAwcHttp(true);
      }
     }
     executingAwc = awc->getExecutingAwc();
     if(executingAwc){
      Serial.println("calling doWaterChange()");
       atoAwcUtil->webSerialWrite("EAWC_4,");
       awc->doWaterChange();

        //doWaterChange();
      }
      Serial.println("after awcNot");
     //delay(1000);
     ///////////////////////////////////////////////////
     //
     // Main loop fuction for ATO
     //
     /////////////////////    ATO timer  ///////////////////////////////
     
      //bool functionAto = timer->executeTest("skippy", 300000);
      bool functionAto = timer->execute("atoNot");
      //bool functionAto = false;
     if(functionAto){
      executingAto = true;
      ato->setExecutingAto(true);
      functionAto = false;
      if(!ato->getSentAtoHttp() && ato->getItRan()){
        sendHttp("Auto_Top_Off");
        sentAtoHttp = true;
        ato->setSentAtoHttp(true);
        ato->setItRan(false);
      }
     }
     executingAto = ato->getExecutingAto();
     if(executingAto){
      Serial.println("calling doAto()");
      atoAwcUtil->webSerialWrite("EATO_5,");
      ato->doAto();

        //doAto();
      }
      Serial.println("after atoNot");
     //delay(1000);
     ///////////////////////////////////////////////////
     //
     // Main loop fuction for Heartbeat
     //
     ////////////////////////////////////////////////////
     bool fuctionHb = timer->execute("hbNot");
     if(fuctionHb){
      Serial.println("executing hbNot");      
      atoAwcUtil->webSerialWrite("EHB_6,"); 
      sendHttp("Heartbeat");
     }
     Serial.println("after hbNot");
     bool fuctionBD = timer->execute("bdNot");
       if(fuctionBD){
        Serial.println("executing blueDoser");      
        atoAwcUtil->webSerialWrite("EBD_7,");
        sendHttp("Blue_Dosing");
        dosing->dose("blue");
     }
     Serial.println("after bdNot");
     bool fuctionGD = timer->execute("gdNot");
     if(fuctionGD){
        Serial.println("executing greenDoser");      
        atoAwcUtil->webSerialWrite("EGD_8,");
        sendHttp("Green_Dosing");
        dosing->dose("green");
     }
     Serial.println("after gdNot");
     bool fuctionYD = timer->execute("ydNot");
       if(fuctionYD){
        Serial.println("executing yellowDoser");      
        atoAwcUtil->webSerialWrite("EYD_9,");
        sendHttp("Yellow_Dosing");
        dosing->dose("yellow");
     }
     Serial.println("after ydNot");
     //just to test doser
  /*sensorValue0 = 1;
  Serial.println("Motor 1 on");
  sendJson();
  //delay(10000);
  sensorValue0 = 0;
  Serial.println("Motor 1 off");
  sendJson();
  delay(5000);
  sensorValue1 = 1;
  Serial.println("Motor 2 on");
   sendJson();
  delay(10000);
  sensorValue1 = 0;
  Serial.println("Motor 2 off");
  sendJson();
  delay(5000);
  sensorValue2 = 1;
  Serial.println("Motor 3 on");
  sendJson();
  delay(10000);
  sensorValue2 = 0;
  Serial.println("Motor 3 off");
  sendJson();
  delay(5000);*/

 
  }else{
    String timezone = atoAwcUtil->readFile(SPIFFS, "/timezone.txt");
    //Serial.println("&&&&&&&&&&&&&&&&&&&&&&&&timezone from spiffs is "+timezone);
    if(timezone == ""){
      timezoneSet = false;//don't do anything just loop until timezone set
      Serial.print("@");
      webSerialCounter1++;
      if(webSerialCounter1 == 5){
        atoAwcUtil->webSerialWrite("@,");
        webSerialCounter1 = 0;
      }
    }else{
          /////////////  just setting timezone///////////////////
          String yourTz = atoAwcUtil->readFile(SPIFFS, "/timezone.txt");
            char tz[yourTz.length()];   
            int i; 
            for (i = 0; i < sizeof(tz); i++) { 
                tz[i] = yourTz[i]; 
            }
          atoAwcUtil->setTimeZone(yourTz);
          /////////////////////////////

      timezoneSet = true;
    }
       
  }
  //printWebValue();
  Serial.print("!");
  webSerialCounter2++;
  if(webSerialCounter2 == 300){
    atoAwcUtil->webSerialWrite("!,");
    webSerialCounter2 = 0;
  }
  //printWebValue();
  //if(TEST_MODE){
    //(); //TODO comment this out for prod...this is faking graph data
 // }
 delay(500);   

///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//     water change for interrupt                      //
//////////////////////////////////////////////////////////
/*    if(waterChange == true){
      doWaterChange();
    }

*/
  }/////////////END of Loop//////////////////////////////////////////
}
        
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
////////////////////////////////////////////////////////////////////
//
//  Fuction: readFile
//
//  Input:  SPIFFS, 
//          char[]  filename path i.e. "/String.txt"
//
//  Output:  String of what was strored
//
//  Discription:  Stores a string in the /path in SPIFFS
//
/////////////////////////////////////////////////////////////////////
String readFile(fs::FS &fs, const char * path) {
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    //Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  //Serial.println(fileContent);
  file.close();
  return fileContent;
}

////////////////////////////////////////////////////////////////////
//
//  Fuction: writeFile
//
//  Input:  SPIFFS, 
//          char[] filename path i.e. "/String.txt"
//          String to store
//
//  Output:  String of what was strored
//
//  Discription:  Stores a string in the /path in SPIFFS
//
/////////////////////////////////////////////////////////////////////
void writeFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Writing file: %s\r\n", path);
  //Serial.print("path is : ");
  //Serial.println(path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    atoAwcUtil->webSerialWrite("?EW"); //TODO change to right code
    return;
  }
  if (file.print(message)) {
    //Serial.println("- file written");
      if((strcmp(path,"/timezone.txt")== 0)){
    //Serial.println("Timezone changed!!!!!!!!11");
  }

  } else {
    Serial.println("- write failed");
  }
}
/////////////////////////////////////////////////////////////////////////
//
//web values saved
// 
///////////////////////////////////////////////////////////////////
String processor(const String& var) {
  //Serial.println("-------------------------- "+var);
  if (var == "ifttt") {
    return atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");
  }
  else if (var == "awcfreq") {
    return atoAwcUtil->readFile(SPIFFS, "/awcfreq.txt");
  }
    else if (var == "leaknotfreq") {
      return atoAwcUtil->readFile(SPIFFS, "/leaknotfreq.txt");
    }
    else if (var == "lwnotfreq") {
      return atoAwcUtil->readFile(SPIFFS, "/lwnotfreq.txt");
    }
    else if (var == "hwnotfreq") {
      return atoAwcUtil->readFile(SPIFFS, "/hwnotfreq.txt");
    }
    else if (var == "hbnotfreq") {
      return atoAwcUtil->readFile(SPIFFS, "/hbnotfreq.txt");
    }
    else if (var == "atofreq") {
      return atoAwcUtil->readFile(SPIFFS, "/atofreq.txt");
    }
    else if (var == "atotod") {
      return atoAwcUtil->readFile(SPIFFS, "/atotod.txt");
    }
    else if (var == "leaklevel") {
      return atoAwcUtil->readFile(SPIFFS, "/leaklevel.txt");
    }
    else if (var == "ato") {
      return atoAwcUtil->readFile(SPIFFS, "/ato.txt");
    }
    else if (var == "timezone") {
      return atoAwcUtil->readFile(SPIFFS, "/timezone.txt");
    }
    else if (var == "example") {
      return atoAwcUtil->readFile(SPIFFS, "/dial.txt");
    }
    else if (var == "SLIDER1") {
      //Serial.println("in read of slider1.txt");
      return atoAwcUtil->readFile(SPIFFS, "/slider1.txt");
    }
    else if (var == "SLIDER2") {
      //Serial.println("in read of slider2.txt");
      return atoAwcUtil->readFile(SPIFFS, "/slider2.txt");
    }
    else if (var == "SLIDER3") {
      //Serial.println("in read of slider3.txt");
      return atoAwcUtil->readFile(SPIFFS, "/slider3.txt");
    }
    else if (var == "bdnotfreq") {
      //Serial.println("in read of bd.txt");
      return atoAwcUtil->readFile(SPIFFS, "/bdnotfreq.txt");
    }
    else if (var == "gdnotfreq") {
      //Serial.println("in read of gd.txt");
      return atoAwcUtil->readFile(SPIFFS, "/gdnotfreq.txt");
    }
    else if (var == "ydnotfreq") {
      //Serial.println("in read of yd.txt");
      return atoAwcUtil->readFile(SPIFFS, "/ydnotfreq.txt");
    }
    return String();
  }


 

//////////////////////////////////////////////////////////////
//                                                          //
//   sentHttp                                               //
//                                                          //
//   input: String message                                  //
//                                                          //
//   output: int (not used)                                 //
//                                                          //
//   description:  this uses the ifttt service to send      //
//                  push notifications                      //
//              
//                                                          //
//////////////////////////////////////////////////////////////
int sendHttp(String event) {
    int ret = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //Serial.print("*");
    }

    //Serial.println("");
   // Serial.println("WiFi connected");
    //Serial.println("IP address: ");
   // Serial.println(WiFi.localIP());


    //Serial.print("connecting to ");
    //Serial.println(host + url);
    //  client.setInsecure();
    if (!client.connect(host, httpsPort)) {

      Serial.println("connection failed");
      return 0;
    }
    //Serial.print("requesting URL: ");
  String iftt = atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");

  if(event == "Email"){
    //Serial.println("sending email");
    //url = "/trigger/" + event + "/with/key/"+iftt;//+",params={ \"value1\" : \""+iPAddress+"\", \"value2\" : \"02\", \"value3\" : \"03\" }";
    //Serial.println(url);
    //https://maker.ifttt.com/trigger/garage_deur/with/key/d56e34gf756/?value1=8&value2=5&value3=good%20morning
      //TESTING JSON CREATION
        url = "/trigger/" + event + "/with/key/"+iftt;
        //Serial.println("Starting JSON");
        StaticJsonDocument<80> jdata;
//        StaticJsonBuffer<69> jsonBuffer;
        String json = "{ \"value1\" : \"atoawc ip: "+iPAddress+"\", \"value2\" : \", atoawc hotspot pw: ato_awc_\", \"value3\" : \", doser hotspot pw : yourdoser\" }";
        auto error = deserializeJson(jdata, json);
//        JsonObject& root = jsonBuffer.parseObject(json);
          //Serial.println("TESTING POST");
        
client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 //"Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + json.length() + "\r\n" +
                 "\r\n" + // This is the extra CR+LF pair to signify the start of a body
                 json + "\n");
  }else{

    //url = "/trigger/"+event+"/with/key/bOZMgCFy7Bnhee7ZRzyi19";
    url = "/trigger/" + event + "/with/key/"+iftt;
  
    //Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    //Serial.println("request Sent");
  }
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
       // Serial.println("headers received");
        break;
      }
    }
    String line = client.readStringUntil('\n');

    //Serial.println("reply was:");
    //Serial.println("==========");
    //Serial.println(line);
    //Serial.println("==========");
    //Serial.println("closing connection");
    ret = 1;
    return ret;
    
}

//////////////////////////////////////////////////////////////
//                                                          //
//   ISRoutine                                          //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   description:  this throws and interrupt when awc       //
//                  button is pressed (small indented       //
//                  button on side of device                //
//                                                          //
//////////////////////////////////////////////////////////////
void ISRoutine () {
    //delay(1000);
   int value;
   Serial.println("I am in ISR");
   atoAwcUtil->webSerialWrite("In ISR");
//TODO put this back in   functionAwc = true;//this will start doWaterChange() just like timer
}



float calcPadded(float percent,unsigned long value){
  float returnVal = 0;
  returnVal = (value*percent)+value;
  return returnVal;
}

long awcToMilli(String webInput){
  long milli = 0;
  return milli;
}


/*int getMonth(){
 //Month names
  String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  //Serial.print("Month name: ");
  //Serial.println(currentMonthName);
  return currentMonth;
}

int getDay(){
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  String weekDay = weekDays[timeClient.getDay()];
  //Serial.print("Week Day: ");
  //Serial.println(weekDay);
  if(weekDay.equals("Sunday")){
    return 1;    
  }else if (weekDay.equals("Monday")){
    return 2;
  }else if (weekDay.equals("Tuesday")){
    return 3;
  }else if (weekDay.equals("Wednesday")){
    return 4;
  }else if (weekDay.equals("Thursday")){
    return 5;
  }else if (weekDay.equals("Friday")){
    return 6;
  }else if (weekDay.equals("Saturday")){
    return 7;
  }
}
//////////////////////////////////////////////////////////////
//                                                          //
//   getHour                                                //
//                                                          //
//    input:  String second,                                //
//            minutes, dayNumber, monthNum,                 //
//            year                                          //  
//                                                          //
//    output: int second minute, day, month year            //
//                                                          //
//    description: prints current info  SECONDS = 0,        //
//                  MINUTES = 1,HOURS = 2,DAY = 3,          //
//                  MONTH = 4, YEAR = 5;                    //
//                                                          //
//////////////////////////////////////////////////////////////
int getDates(int val){
  int retVal = 0;
  //Week Days
  String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    unsigned long epochTime = timeClient.getEpochTime();
  //Serial.print("Epoch Time: ");
  //Serial.println(epochTime);
  
  //String formattedTime = timeClient.getFormattedTime();
 // Serial.print("Formatted Time: ");
  //Serial.println(formattedTime);  

  if(val == 2){  //HOURS
    return timeClient.getHours();
    //Serial.print("Hour: ");
    //Serial.println(currentHour);  
  }
  if(val == 1){  //MINUTES
    return timeClient.getMinutes();
    //Serial.print("Minutes: ");
    //Serial.println(currentMinute); 
  }
  if(val == 0){  //SECONDS
    return timeClient.getSeconds();
    //Serial.print("Seconds: ");
    //Serial.println(currentSecond);  
  }
  //String weekDay = weekDays[timeClient.getDay()];
  //Serial.print("Week Day: ");
 // Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  if(val == 3){  //DAY
    return ptm->tm_mday;
    //Serial.print("Month day: ");
    //Serial.println(monthDay);
  }
  if(val == 4){  //MONTH
    return ptm->tm_mon+1;
    //Serial.print("Month: ");
    //Serial.println(currentMonth);
  }
  //String currentMonthName = months[currentMonth-1];
  //Serial.print("Month name: ");
  //Serial.println(currentMonthName);
  if(val == 5){  //YEAR
    return ptm->tm_year+1900;
    //Serial.print("Year: ");
    //Serial.println(currentYear);
  }
  //Print complete date:
//  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  //Serial.print("Current date: ");
  //Serial.println(currentDate);

  //Serial.println("");
}

*/
void appendFile(String str){
  //
  String atoFile = atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");
}




void printWebValue(){
  Serial.println("Printing web values stored");
  String ift = atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");
  Serial.println(ift);

  String awc = atoAwcUtil->readFile(SPIFFS, "/awcfreq.txt");
  Serial.println(awc);  

  String atoF = atoAwcUtil->readFile(SPIFFS, "/atofreq.txt");
  Serial.println(atoF);  

  String awctod = atoAwcUtil->readFile(SPIFFS, "/atotod.txt");
  Serial.println(awctod);  

  String leak = atoAwcUtil->readFile(SPIFFS, "/leaknotfreq.txt");
  Serial.println(leak);
  
  String lw = atoAwcUtil->readFile(SPIFFS, "/lwnotfreq.txt");
  Serial.println(lw);
  
  String hw = atoAwcUtil->readFile(SPIFFS, "/hwnotfreq.txt");
  Serial.println(hw);
  
  String hb = atoAwcUtil->readFile(SPIFFS, "/hbnotfreq.txt");
  Serial.println(hb);
  
  String bd = atoAwcUtil->readFile(SPIFFS, "/bdnotfreq.txt");
  Serial.println(bd);
  
  String gd = atoAwcUtil->readFile(SPIFFS, "/gdnotfreq.txt");
  Serial.println(gd);
  
  String yd = atoAwcUtil->readFile(SPIFFS, "/ydnotfreq.txt");
  Serial.println(yd);
  
  String leakLevel = atoAwcUtil->readFile(SPIFFS, "/leaklevel.txt");
  Serial.println(leakLevel);
  
  String ato = atoAwcUtil->readFile(SPIFFS, "/ato.txt");
  Serial.println(ato); 

  String tz = atoAwcUtil->readFile(SPIFFS, "/timezone.txt");
  Serial.println(tz);
  atoAwcUtil->setTimeZone(tz);

  String dl = atoAwcUtil->readFile(SPIFFS, "/dial.txt");
  Serial.println(dl);
 // setTimeZone(dl);

  String bCal = atoAwcUtil->readFile(SPIFFS, "/hundredMlBlue.txt");
  Serial.println("Blue 100ml calibration value is "+bCal); 

  String gCal = atoAwcUtil->readFile(SPIFFS, "/hundredMlGreen.txt");
  Serial.println("Green 100ml calibration value is "+gCal); 

  String  yCal = atoAwcUtil->readFile(SPIFFS, "/hundredMlYellow.txt");
  Serial.println("Yellow 100ml calibration value is "+yCal); 
}


float secondsToGallons(int seconds){
  //pumps do 110l or 29 gallons an hour at <0.9 meters
  //3600 sec in an hour
  //0.0081 gal/sec
  float GALPERSEC = 0.0081;
  float retVal = GALPERSEC*seconds;
  return retVal;
}

int secondsToMilliliters(int seconds){
  //pumps do 110l or 29 gallons an hour at <0.9 meters
  //3600 sec in an hour
  //0.0081 gal/sec
  float MLPERSEC = 0.04;//TODO figure this out
  int retVal = MLPERSEC*seconds;
  return retVal;
}


//////////////////////////////////////////////////////////////
//                                                          //
//   startWifi                                              //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   starts the wifi manager and mdns then reboots          //
//                                                          //
//////////////////////////////////////////////////////////////

void startWifi(){
    unsigned long startedAt = millis();
 /*   
    WiFi.mode(WIFI_STA);
     
    WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
    Serial.println("Opening configuration portal");
    //digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
    //Local intialization. Once its business is done, there is no need to keep it around
      //start-block2
  //IPAddress _ip = IPAddress(192,168, 1, 200);
  //IPAddress _gw = IPAddress(192, 168, 1, 1);
 // IPAddress _sn = IPAddress(255, 255, 255, 0);
  //end-block2
  
    //WiFiManager wifiManager;
  //wifiManager.resetSettings();
  //set custom ip for portal
  //wifiManager.setAPStaticIPConfig(IPAddress(192,168,1,44), IPAddress(10,0,1,1), IPAddress(255,255,255,0));//  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
    //sets timeout in seconds until configuration portal gets turned off.
    //If not specified device will remain in configuration mode until
    //switched off via webserver.
    if (WiFi.SSID() != "") wifiManager.setConfigPortalTimeout(60); //If no access point name has been previously entered disable timeout.

    //it starts an access point
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.startConfigPortal("ATOAWC", "ato_awc_")) { //Delete these two parameters if you do not want a WiFi password on your configuration access point
      Serial.println("Not connected to WiFi but continuing anyway.");
 //     WebSerial.println("hotspot ip address is http://192.168.4.1/");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
            delay(1000);
//      ESP.reset();
    }
    //digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
    // For some unknown reason webserver can only be started once per boot up
    // so webserver can not be used again in the sketch.
//  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

    Serial.print("After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis() - startedAt);
    Serial.print(waited / 1000);
    Serial.print(" secs in setup() connection result is ");
    Serial.println(connRes);
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("failed to connect, finishing setup anyway");
    } else {
      Serial.print("local ip: ");
      Serial.println(WiFi.localIP());
      WiFi.mode(WIFI_STA);
      server.begin();////######server begin
    }
 /*     if(!MDNS.begin("atoawc")) {
     Serial.println("Error starting mDNS");
     return;
  }
//    MDNS.addService("http", "tcp", 80);
     //WiFi.persistent(false)
    server.begin();*/
}


//////////////////////////////////////////////////////////////
//                                                          //
//   startSpiffs                                            //
//                                                          //
//   input: none                                            //
//                                                          //
//   output: none                                           //
//                                                          //
//   description:  starts of the file disk system.          //
//                                                          //
//////////////////////////////////////////////////////////////
void startSpiffs(){
      if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

}
