//TODO Restart everything if you change timezone...call new and delete old DateTime
//TODO if Timer fails to set time in DateTime stop
//TODO if timer set or update fail they go into loop
//  should return false and go into save mode
//  only run things that don't depend on time
//    low water, high water, etc
//TODO change web setting while running
//TODO recompile Client if(!WiFi.connected) using proper board and load on client.
//  reset stuff not.txt = blank "",  and spiffsReset = 1

#include <WiFi.h>


//needed for library
//#include "WiFiManager.h"
//WiFiManager wifiManager;


//#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>                     //this is SPIFFS file system
#include "SD.h"   SD card
#include "SPI.h"  SD card
//////////////////////////////Include LIbraries ////////////////////
#include <atoawc_util.h>
#include <timer.h>
#include <ato.h>
#include <awc.h>
#include <dosing.h>
#include <graphs.h>
#include <db.h>

////////////////////////////////////////////////////////////////////
//int BUILTIN_LED = 25;

//////////Library pointers //////////////////////////////////
////////////////////Pointer stuff///////////////////
AtoAwcUtil *atoAwcUtil;
Testharness *testharness;
Timer *timer;
Ato *ato;
Awc *awc;
Graphs *graphs;
Dosing *dosing;
Db *db;
/////////////////////////////////////////////////////////////

///////////////////////// Variables ///////////////////////////////////
WiFiClient client;  //this is passed into Dosing constructor for connecting to doser
String markKey1 = "fBplW8jJqqotTqTxck4oTdK_oHTJKAawKfja-WlcgW-";
const char* markKey = markKey1.c_str();
String ericKey1 = "bOZMgCFy7Bnhee7ZRzyi19";
const char* ericKey = ericKey1.c_str();
////////////////////// Web Servier Variables //////////////////////////////
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
String jsonAto;
String jsonYearAto;
String jsonYearAwc;
String jsonYearBlue;
String jsonYearGreen;
String jsonYearYellow;
///////////////////////////////////////////////////////////////////////////

void sendJson();
bool DEBUG = true;
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

const char* host = "maker.ifttt.com";  //used in sendHttp
const int httpsPort = 80;  //used in sendHttp
String url = "";  //used in sendHttp
String iPAddress;  //used in sendHttp

bool calibrating = false;
int oneHundredmlBlueTime = 0;
int oneHundredmlGreenTime = 0;
int oneHundredmlYellowTime = 0;
bool blueStarted = false;
bool greenStarted = false;
bool yellowStarted = false;
int webSerialCounter1 = 0;
int webSerialCounter2 = 0;
unsigned long atoStart = 0, atoStop = 0, awcStart = 0, awcStop = 0, yellowStart = 0, yellowStop = 0;
unsigned long  greenStart = 0, greenStop = 0, blueStart = 0, blueStop = 0;

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
bool firstTime = false;

int sentWLCount;
int sentWHCount;
int sentLeakCount;
bool timezoneSet = false;
bool sentAtoHttp = false;
bool sentAwcHttp = false;
bool sentBlueHttp = false;
bool sentGreenHttp = false;
bool sentYellowHttp = false;

bool atoRunning = false;
bool awcRunning = false; //TODO use this

bool onBoardLightOn = false;

//testing only for awc
int AWCTESTTIME = 3600000;
bool _awcFirstTime = true;
long _awcTime = 0;
long _awcNowTime = 0;

//////////////////////////////// end of Variables//////////////////////////////////

///////////////////////////// Functions /////////////////////////////////////////
void configModeCallback (AsyncWiFiManager *myWiFiManager);
bool initSDCard();  //this is for SD card
String processor(const String& var);  //used for Webserver to process request
void notFound(AsyncWebServerRequest *request);  //used for
int sendHttp(String message);  // sending email
float secondsToGallons(int seconds);
int secondsToMilliliters(int seconds);

///////////////////////////// end of Functions //////////////////////////////////



//gets called when WiFiManager enters configuration mode

AsyncWebServer server(80);
DNSServer dns;

void setup() {  ///////////////////// START OF SETUP ////////////////////////////////
  // put your setup code here, to run once:
  Serial.begin(115200);
//  startSpiffs();

  bool cardMounted = initSDCard();
  bool mountingCard = true;
  while(mountingCard){
    if(!cardMounted){
      mountingCard = true;
      cardMounted = initSDCard();
      delay(5000);
      Serial.print("looping in card mount");
    }else{
      mountingCard = false;
    }
  }
  //////////////////////////Set up pins //////////////////////////////
  pinMode(5, OUTPUT);
  ////////////////////////////////////////////////////////////////////
  //Serial.print("FreeHeap is :");
  //Serial.println(ESP.getFreeHeap());

  ///////////////////Start WiFi ///////////////////////////////////////
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server, &dns);
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("ATOAWC")) {
    Serial.println("failed to connect and hit timeout");
    Serial.println("restarting esp");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }
  delay(50);
  //Serial.print("FreeHeap is :");
  //Serial.println(ESP.getFreeHeap());
  delay(50);
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  //////////////////////////////////////////////////////////////////////////////////

  //////////////////////Create Libraries ///////////////////////////////////////
  atoAwcUtil = new AtoAwcUtil(&server);
  //TODO remove this after testing
  //TODO this is just to start the testharness evap
  delay(100); 
  
  /////////////////////////////////////////////////////////////////////////////
  atoAwcUtil->beginUtil();// have to start  DON'T FORGET THIS
  //////////////////remove this after testing or add all of them /////////////
  /*String blank = "";
  atoAwcUtil->writeFile(SPIFFS, "/timezone.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/leakNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/hwNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/lwNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/atoNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/awcNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/hbNot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/bdnot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/gdnot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/ydnot.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/leaknotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/hwnotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/lwnotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/atofreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/hbnotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/bdnotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/gdnotfreq.txt", blank.c_str());
  delay(100);
  atoAwcUtil->writeFile(SPIFFS, "/ydnotfreq.txt", blank.c_str());
  delay(100);*/
  
  timer = new Timer(atoAwcUtil);
  testharness = new Testharness();
  ato = new Ato(atoAwcUtil, testharness, DEBUG);
 //ato->beginAto();
  awc = new Awc(atoAwcUtil,testharness, DEBUG);
  graphs = new Graphs(atoAwcUtil);
  dosing = new Dosing(atoAwcUtil, client);
  db = new Db();
  //TODO remove this for permanent produciton
  //ALSO need to fix unplugging and plugging back in timers
  atoAwcUtil->clearSpiffs();
  //TODO
  //give AtoAwcUtil a new Datetime
  //sppiff get offset 
  //String offset = readFile(SPIFFS, "/offset.txt");
  //if offset not "" empty then
  //bool dst = _atoAwcUtil->getIsDST();
  //Datetime* _datetime = new Datetime(offset,dst);
  //AtoAwcUtil->setDatetime(Datetime* _datetime);
  //////////////////////////////////

  ////////////key work//////////////////////////////////////////////////////////////
  String ift = atoAwcUtil->readFile(SPIFFS, "/ifttt.txt");
  //Serial.print("ift key is ");
  //Serial.println(ift);
  if (ift.length() <= 5) {
    atoAwcUtil->writeFile(SPIFFS, "/ifttt.txt", markKey);
    //Serial.println("In write ifttt key");
  }
  ///////////////////////////////////////////////////////////////////////////////

  /////////////// Beginning of Web Server Setup //////////////////////////////////////
Serial.println("i got here 1");
  // Send web page with input fields to client
   server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
     request->send(SD, "/index.html", String(),false,processor);
    });
  ///////////////////////////////////////////////////////////////////
 /* server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/index.html", String(), false, processor);
  });*/

  ////////////////////////////////////////////////////////////////////

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
      String webSaved = atoAwcUtil->readFile(SPIFFS,"/atofreq.txt");
      Serial.print("/atofeq.txt is : ");
      Serial.println(inputMessage);
      Serial.println(webSaved);
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
      Serial.println("just wrote timezone :" + inputMessage);
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
  server.on("/jsonAto", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json",  jsonYearAto);
  });
  server.on("/jsonAwc", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json",  jsonYearAwc);
  });
  server.on("/jsonBlue", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json",  jsonYearBlue);
  });
  server.on("/jsonGreen", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json",  jsonYearGreen);
  });
  server.on("/jsonYellow", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json",  jsonYearYellow);
  });

  server.on("/morish.jpg", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/morish.jpg", "image/jpg");
  });

  // Route for root / web page
  server.on("/chart", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/charts.html");
  });

  // Route for root / web page
  server.on("/doser", HTTP_GET, [](AsyncWebServerRequest * request) {
   // Serial.println("We are in /doser at leat!!!!!!!!!!!");
    request->send(SD, "/doser.html", String(),false,processor);
  });
  
  server.on("/package", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/package.json");
  });

  server.on("/pureknob.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SD, "/pureknob.js");
  });

  server.onNotFound(notFound);
  server.begin();  /////////probably have to change THIS

  //////////////////////Doser//////////////////////////////////////////
  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT1)) {
      inputMessage = request->getParam(PARAM_INPUT1)->value();
      slider1 = inputMessage;
      //atoAwcUtil->writeFile(SPIFFS, "/awcfreq.txt", inputMessage.c_str());
      atoAwcUtil->writeFile(SPIFFS, "/slider1.txt", slider1.c_str());
      //Serial.println("slider value 1 izzzzzzzzzzzzzzzzzzzzzs "+slider1);
      atoAwcUtil->webSerialWrite("Blue slider value is " + slider1);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT2)) {
      inputMessage = request->getParam(PARAM_INPUT2)->value();
      slider2 = inputMessage;
      atoAwcUtil->writeFile(SPIFFS, "/slider2.txt", slider2.c_str());
      Serial.println("slider value 2 is " + slider2);
      atoAwcUtil->webSerialWrite("Green slider value is " + slider2);

      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT3)) {
      inputMessage = request->getParam(PARAM_INPUT3)->value();
      slider3 = inputMessage;
      atoAwcUtil->writeFile(SPIFFS, "/slider3.txt", slider3.c_str());
      Serial.println("slider value 3 is " + slider3);
      atoAwcUtil->webSerialWrite("Yellow slider value is " + slider3);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox1", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT1)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT1)->value();
      chkboxValue1 = inputMessage;
      if (chkboxValue1 == "true") {
        ckBox1 = true;
      } else if (chkboxValue1 = "false") {
        ckBox1 = false;
      }
      Serial.println("checkbox value 1 is " + chkboxValue1);
      atoAwcUtil->webSerialWrite("Blue checkbox value is " + chkboxValue1);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox2", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT2)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT2)->value();
      chkboxValue2 = inputMessage;
      if (chkboxValue2 == "true") {
        ckBox2 = true;
      } else if (chkboxValue2 = "false") {
        ckBox2 = false;
      }
      Serial.println("checkbox value 2 is " + chkboxValue2);
      atoAwcUtil->webSerialWrite("Green checkbox value is " + chkboxValue2);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/chbox3", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CHKBOXINPUT3)) {
      inputMessage = request->getParam(PARAM_CHKBOXINPUT3)->value();
      chkboxValue3 = inputMessage;
      if (chkboxValue3 == "true") {
        ckBox3 = true;
      } else if (chkboxValue3 = "false") {
        ckBox3 = false;
      }
      Serial.println("checkbox value 3 is " + chkboxValue3);
      atoAwcUtil->webSerialWrite("Yellow checkbox value is " + chkboxValue3);
      //analogWrite(output, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  Serial.println("i got here 2");

  //////////////////////////////// end of Web Server Setup //////////////////////////////////////
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
  server.begin();
  //.begin(&server);
  //.msgCallback(recvMsg);
  sendHttp("Email");//TODO what is this
  //send_webhook("email","fBplW8jJqqotTqTxck4oTdK_oHTJKAawKfja-WlcgW-","Hi its Wemos!","value 2","value 3");
    timer->setSpiffsLReset(1);
    timer->setSpiffsHWReset(1);
    timer->setSpiffsLWReset(1);
    timer->setSpiffsAtoReset(1);
    timer->setSpiffsAwcReset(1);
    timer->setSpiffsHbReset(1);
    timer->setSpiffsBdReset(1);
    timer->setSpiffsGdReset(1);
    timer->setSpiffsYdReset(1);

Serial.println("going to run beginAto");
    ato->beginAto();
Serial.println("ran beginAto");



   
}  ////////////////////// END OF SETUP///////////////////////////////////////
int xx = 0;
void loop() { /////////////////////// START OF LOOP ////////////////////////
/*   int shit = digitalRead(22);
   Serial.print("Twenty two is : ");
   Serial.println(shit);*/
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("LOST WiFi restarting");
    //atoAwcUtil->webSerialWriteLine("LOST WiFi restarting");
    ESP.restart();
  }

  //calibration has top priority
  if (ckBox1 || ckBox2  || ckBox3 || calibrating) {

    calibrating = true;
    dosing->setCalibrating(true);
    dosing->setCkBox1(ckBox1);
    dosing->setCkBox2(ckBox2);
    dosing->setCkBox3(ckBox3);
    dosing->calibrate();
    calibrating = dosing->getCalibrating();//TODO need to test this

  } else {
    calibrating = false;
    if (WiFi.status() != WL_CONNECTED)
    {
      //startWifi();
      ESP.restart();
    }

    // MDNS.update();
    //delay(1000);
    //timeClient.update();
    //.print("hs 192.168.4.1/, ato_awc_");
    String ipa = WiFi.localIP().toString();
    //.print(ipa+",");
    //.println("!");
    //delay(1000);
    //Serial.println(s3);
    // put your main code here, to run repeatedly:
    /*  if(WiFi.status() != WL_CONNECTED){
        Serial.println("Reseting");  //TODO do we need this?
        .println("Reseting");
        ESP.restart();
      }else{
        //Serial.print(".");
      }*/
    if (firstTime) {
      //This just send text ESP is running
      //sendHttp("ESP");
      sendHttp("ATO_AWC_Started");

      //Serial.println("trying to send ESP");
      //      int status = ("ESP");
      firstTime = false;
      timer->setYrMoDay();
    }
    ///////////////////////////////main timers looping/////////////////
    if (timezoneSet) { // don't do anything until timezone is set
      /////// time stuff //////////////////////////////////////////////
      ////////**** use this timezone stuff**************////////////////
      // Init and get the time
      //timeClient.begin();
      int setYear = 2020;
      int* timeArr;
      while(setYear <= 2020 || setYear >2040){
        timeArr = atoAwcUtil->setCurrentTime(); //TODO store current day in SPIFFS
        setYear = timeArr[0];
        if(setYear <= 2020 || setYear >2040){
          Serial.print("ERROR we can't start until we get a good time IN LOOP");
          atoAwcUtil->webSerialWrite("X_NOTIME, ");
          delay(30000);
        }
      /*  Serial.print("Year is : ");
        Serial.println(timeArr[0]);
        Serial.print("Month is : ");
        Serial.println(timeArr[1]);
        Serial.print("Day is : ");
        Serial.println(timeArr[2]);
        Serial.print("Hour is : ");
        Serial.println(timeArr[3]);
        Serial.print("Minute is : ");
        Serial.println(timeArr[4]);*/

      }
      timer->setYrMoDay();// this is used to see if day changed from now
      timer->setCurrentDay();

      

      //timer->setCurrentTime();
      bool fuctionNewDay = timer->execute("dayChange");
      //fuctionNewDay = true;
      if (fuctionNewDay) {
        Serial.println("executing new day");
        atoAwcUtil->webSerialWrite("$_NEWDAT, ");
        
        atoAwcUtil->webSerialWrite("executing new day");
        String daileyatoFill = atoAwcUtil->readFile(SPIFFS, "/atodailyfill.txt");
        int atoDFill = daileyatoFill.toInt();
        float valato = secondsToGallons(atoDFill);
        Serial.println("ato daily gallon is " + String(valato));
        String daileyStr = "0";
        atoAwcUtil->writeFile(SPIFFS, "/atodailyfill.txt", daileyStr.c_str());
        graphs->closeDayPutInArr("ato", valato, 0, 0, 0); //the 0,0 is for test mode
        String daileyawcFill = atoAwcUtil->readFile(SPIFFS, "/awcdailyfill.txt");
        int awcDFill = daileyawcFill.toInt();
        float valawc = secondsToGallons(awcDFill);
        Serial.println("awc daily gallon is " + String(valawc));
        daileyStr = "0";
        atoAwcUtil->writeFile(SPIFFS, "/awcdailyfill.txt", daileyStr.c_str());
        graphs->closeDayPutInArr("awc", valawc, 0, 0, 0);
        //NOTE dosing already in ml!!!!
        String daileybdFill = atoAwcUtil->readFile(SPIFFS, "/bddailyfill.txt");
        float bdDFill = daileybdFill.toInt();
        Serial.println("blue dose daily ml is " + String(bdDFill));
        daileyStr = "0";
        atoAwcUtil->writeFile(SPIFFS, "/bddailyfill.txt", daileyStr.c_str());
        graphs->closeDayPutInArr("blue", bdDFill, 0, 0, 0);
        String daileygdFill = atoAwcUtil->readFile(SPIFFS, "/gddailyfill.txt");
        float gdDFill = daileygdFill.toInt();
        Serial.println("green dose daily ml is " + String(gdDFill));
        daileyStr = "0";
        atoAwcUtil->writeFile(SPIFFS, "/gddailyfill.txt", daileyStr.c_str());
        graphs->closeDayPutInArr("green", gdDFill, 0, 0, 0);
        String daileyydFill = atoAwcUtil->readFile(SPIFFS, "/yddailyfill.txt");
        float ydDFill = daileyydFill.toInt();
        Serial.println("yellow dose daily ml is " + String(ydDFill));
        daileyStr = "0";
        atoAwcUtil->writeFile(SPIFFS, "/yddailyfill.txt", daileyStr.c_str());
        graphs->closeDayPutInArr("yellow", ydDFill, 0, 0, 0);
        //timer->setYrMoDay();//do this in timer now
        //yrMoDay[0] = currentTimeArr[0];  //FINALLY CHANGE TO THE NEW DAY After writing array
        //yrMoDay[1] = currentTimeArr[1];
        //yrMoDay[2] = currentTimeArr[2];
        Serial.println("finished newDay");
      }
      //////////new with lights for sensor ////////////////////

      ato->doWaterLights();

      //Serial.println("after doWaterLIghts()");


      ////////////////////////////////////////////////////////
      bool fuctionLeak = timer->execute("leakNot");
      if (fuctionLeak) {
        Serial.println("executing leak");
        atoAwcUtil->webSerialWrite("EL_1,");
        //sendHttp("Leak");
        if (ato->checkLeaks()) {
          sendHttp("Leak");
          atoAwcUtil->webSerialWrite("$_EL_1_A_Leak, ");
        }
      }
//      Serial.println("after leakNot");
      //delay(1000);
      bool fuctionHW = timer->execute("highWaterNot");
      if (fuctionHW) {
        Serial.println("executing high water");
        atoAwcUtil->webSerialWrite("EHW_2,");
        //sendHttp("HighWater");
        if (ato->checkHighWater()) {
          atoAwcUtil->webSerialWrite("EHW_2_A_High water_!, ");
          sendHttp("HighWater");
        }
      }
//      Serial.println("after highWaterNot");
      //delay(10000);
      bool fuctionLW = timer->execute("lowWaterNot");
      if (fuctionLW) {
        Serial.println("executing low water");
        atoAwcUtil->webSerialWrite("ELW_3,");
        //sendHttp("LowWater");
        if (ato->checkLowWater()) {
          sendHttp("LowWater,");
          atoAwcUtil->webSerialWrite("ELW_3_A_Low water_!, ");
        }
      }
//      Serial.println("after lowWaterNot");
      //delay(1000);
      ato->setWaterChange(false);
      /////////////////////    timer  ///////////////////////////////
if(DEBUG){
  timer->execute("awcNot"); //set up just don't use the return.
  if(_awcFirstTime){
    _awcFirstTime = false;
    _awcTime = millis();
  }else{
    _awcNowTime = millis() - _awcTime;
    if(_awcNowTime > AWCTESTTIME){
      functionAwc = true;
      _awcTime = millis();
      _awcNowTime = 0;
      _awcFirstTime = true;
    }
  }
}else{
      functionAwc = timer->execute("awcNot");
}
      //bool functionAwc = timer->executeTest("skippy", 900000);
      //bool functionAwc = false;
      if (functionAwc) {
        Serial.println("executing AWC");
/////!!!!!!!!!!!!TODO this is ALL SCREWED UP ///////////////
////////////////////////////////////////////////////////////
        executingAwc = true;
        awc->setExecutingAwc(true);
        awc->setWaterChange(true);
        awc->setWaterEmptied(false);
        functionAwc = false;
        if (!awc->getSentAwcHttp()) {
          sendHttp("Auto_Water_Change");
          sentAwcHttp = true;
          awc->setSentAwcHttp(true);
        }
      }
      executingAwc = awc->getExecutingAwc();
      if (executingAwc) {
        Serial.println("calling doWaterChange()");
//        atoAwcUtil->webSerialWrite("EAWC_4,");
        awc->doWaterChange();

        //doWaterChange();
      }
      // This gets removed
    
//      Serial.println("after awcNot");
      //delay(1000);
      ///////////////////////////////////////////////////
      //
      // Main loop fuction for ATO
      //
      /////////////////////    ATO timer  ///////////////////////////////

      //bool functionAto = timer->executeTest("skippy", 300000);
      bool functionAto = timer->execute("atoNot");
      //bool functionAto = false;
      if (functionAto) {
        Serial.println("Execution ATO");
        //////////////////////This is messed up too
        executingAto = true;
        ato->setExecutingAto(true);
        functionAto = false;
        if (!ato->getSentAtoHttp() && ato->getItRan()) {
          sendHttp("Auto_Top_Off");
          sentAtoHttp = true;
          ato->setSentAtoHttp(true);
          ato->setItRan(false);
        }
      }
      executingAto = ato->getExecutingAto();
      if (executingAto) {
        Serial.println("calling doAto()");
        atoAwcUtil->webSerialWrite("EATO_5,");
        ato->doAto();

        //doAto();
      }
       //this gets removed
//      Serial.println("after atoNot");
      //delay(1000);
      ///////////////////////////////////////////////////
      //
      // Main loop fuction for Heartbeat
      //
      ////////////////////////////////////////////////////
      bool fuctionHb = timer->execute("hbNot");
      if (fuctionHb) {
        Serial.println("executing hbNot");
        atoAwcUtil->webSerialWrite("EHB_6,");
        sendHttp("Heartbeat");
      }
//      Serial.println("after hbNot");
      bool fuctionBD = timer->execute("bdNot");
      if (fuctionBD) {
        Serial.println("executing blueDoser");
        atoAwcUtil->webSerialWrite("EBD_7,");
        sendHttp("Blue_Dosing");
        dosing->dose("blue");
      }
//      Serial.println("after bdNot");
      bool fuctionGD = timer->execute("gdNot");
      if (fuctionGD) {
        Serial.println("executing greenDoser");
        atoAwcUtil->webSerialWrite("EGD_8,");
        sendHttp("Green_Dosing");
        dosing->dose("green");
      }
//      Serial.println("after gdNot");
      bool fuctionYD = timer->execute("ydNot");
      if (fuctionYD) {
        Serial.println("executing yellowDoser");
        atoAwcUtil->webSerialWrite("EYD_9,");
        sendHttp("Yellow_Dosing");
        dosing->dose("yellow");
      }
//      Serial.println("after ydNot");
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


    } else {
      Serial.println("local ip");
      Serial.println(WiFi.localIP());
      String timezone = atoAwcUtil->readFile(SPIFFS, "/timezone.txt");
      //Serial.println("&&&&&&&&&&&&&&&&&&&&&&&&timezone from spiffs is "+timezone);
      if (timezone == "") {
        timezoneSet = false;//don't do anything just loop until timezone set
        Serial.print("@");
        webSerialCounter1++;
        if (webSerialCounter1 == 5) {
          atoAwcUtil->webSerialWrite("@,");
          webSerialCounter1 = 0;
        }
      } else {
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
    if (webSerialCounter2 == 1) {
       atoAwcUtil->webSerialWrite("!,");    }
    if (webSerialCounter2 == 300) {
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
  }/////////////END timezone set//////////////////////////////////////////


  //Serial.print("$");
  if(onBoardLightOn){
    digitalWrite(5, LOW);
    onBoardLightOn = true;
  }else{
    digitalWrite(5, HIGH);
    onBoardLightOn = false;
  }
}  ///////////////////////  END of LOOP /////////////////////////

//////////////////////////////////////////////////////////////
// call back needed for wifi
/////////////////////////////////////////////////////////////
void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  //myWiFiManager->startConfigPortal("ATOAWC");
  //myWiFiManager->autoConnect("ATOAWC");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());

}


/////////////////////////////////////////////////////////////////
//
//  Processor
//
//
////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////
//
//  initSDCard
//
////////////////////////////////////////////////////////////
bool initSDCard(){
  bool retVal = false;
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    atoAwcUtil->webSerialWrite("X_CARDFAILED, ");
    return false;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    atoAwcUtil->webSerialWrite("X_NOCARD, ");
    return false;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  retVal = true;
  return retVal;
}


///////////////////////////////////////////////////////
//
//  notFound
//
////////////////////////////////////////////////////////
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
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

  if (event == "Email") {
    //Serial.println("sending email");
    //url = "/trigger/" + event + "/with/key/"+iftt;//+",params={ \"value1\" : \""+iPAddress+"\", \"value2\" : \"02\", \"value3\" : \"03\" }";
    //Serial.println(url);
    //https://maker.ifttt.com/trigger/garage_deur/with/key/d56e34gf756/?value1=8&value2=5&value3=good%20morning
    //TESTING JSON CREATION
    url = "/trigger/" + event + "/with/key/" + iftt;
    //Serial.println("Starting JSON");
    StaticJsonDocument<80> jdata;
    //        StaticJsonBuffer<69> jsonBuffer;
    String json = "{ \"value1\" : \"atoawc ip: " + iPAddress + "\", \"value2\" : \", atoawc hotspot pw: ato_awc_\", \"value3\" : \", doser hotspot pw : yourdoser\" }";
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
  } else {

    //url = "/trigger/"+event+"/with/key/bOZMgCFy7Bnhee7ZRzyi19";
    url = "/trigger/" + event + "/with/key/" + iftt;

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

float secondsToGallons(int seconds) {
  //pumps do 110l or 29 gallons an hour at <0.9 meters
  //3600 sec in an hour
  //0.0081 gal/sec
  float GALPERSEC = 0.0081;
  float retVal = GALPERSEC * seconds;
  return retVal;
}

int secondsToMilliliters(int seconds) {
  //pumps do 110l or 29 gallons an hour at <0.9 meters
  //3600 sec in an hour
  //0.0081 gal/sec
  float MLPERSEC = 0.04;//TODO figure this out
  int retVal = MLPERSEC * seconds;
  return retVal;
}
