/*
 * Mark Rood
 * 
 * PH and temp probe
 * 
 * Also has OTA programming http://192.168.1.176/firmare
 * 
 * Also has web server to get ph and temp
 *  192.168.1.176/ph
 *  192.168.1.176/temp
 *   
 *   Board is:  HELTEC WiFi Kit 32
 *   
*/

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "heltec.h"
//#include "WiFi.h"
#include "images.h"

#include "DFRobot_ESP_PH.h"
#include "EEPROM.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager
//#include <WiFiManager.h>
#include <WebSerial.h>

#include <Update.h>

#include <OneWire.h>
#include <DallasTemperature.h>




//variabls for blinking an LED with Millis
const int led = 2; // ESP32 Pin to which onboard LED is connected
unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 200;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED

///////////////////////// ph //////////////
DFRobot_ESP_PH ph;
#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300 //the esp voltage supply value
#define PH_PIN 35    //the esp gpio data pin number
float voltage, phValue, temperature = 25;
float temperatureF = 69.0;
const char* host = "PH";

AsyncWebServer server(80);
DNSServer dns;

//WiFiManager wifiManager;
void configModeCallback (AsyncWiFiManager *myWiFiManager);
//String updateIt();
//String readTemp();

/////////////////////////// OTA Web ///////////////////////////////

/////////////////////temp ///////////////////////////////
// GPIO where the DS18B20 is connected to
const int ONE_WIRE_BUS = 17;
OneWire oneWire(ONE_WIRE_BUS);    

// Setup a oneWire instance to communicate with any OneWire devices
//OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
/////////////////////////////////////////////////////////////////
/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Index page */
String indexPage = 
"<h1 style=\"color:white; text-align:center;\">ESP32 Garage Management</h1>" + style;

/* Firmware Page */
String firmwarePage = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;
///////////////////////////////////////////////////////////////////////////
String readPh() {
  return String(phValue);
}
//////////////////////////// ph /////////////////////////////////
String readTemp(){
  return String(temperatureF);
}
////////////////////////////////////////////////// Display //////////////////
void logo(){
  Heltec.display -> clear();
  Heltec.display -> drawXbm(0,5,logo_width,logo_height,(const unsigned char *)logo_bits);
  Heltec.display -> display();
}


void displaySensor(void)
{
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display -> drawString(0, 20, "PH: "+readPh());
  Heltec.display -> display();

  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display -> drawString(0, 30, " ");
  Heltec.display -> display();


  //int n = WiFi.scanNetworks();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display -> drawString(0, 40, "Temp: "+readTemp());
  Heltec.display -> display();
  delay(1000);
  Heltec.display -> clear();
/*
  if (n == 0)
  {
    Heltec.display -> clear();
    Heltec.display -> drawString(0, 0, "no network found");
    Heltec.display -> display();
    while(1);
  }
  else
  {
    Serial.print(n);
    Heltec.display -> drawString(0, 0, (String)n);
    Heltec.display -> drawString(14, 0, "networks found:");
    Heltec.display -> display();
    delay(500);

    for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
      Heltec.display -> drawString(0, (i+1)*9,(String)(i + 1));
      Heltec.display -> drawString(6, (i+1)*9, ":");
      Heltec.display -> drawString(12,(i+1)*9, (String)(WiFi.SSID(i)));
      Heltec.display -> drawString(90,(i+1)*9, " (");
      Heltec.display -> drawString(98,(i+1)*9, (String)(WiFi.RSSI(i)));
      Heltec.display -> drawString(114,(i+1)*9, ")");
      //            display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }*/

  Heltec.display -> display();
  delay(100);
  Heltec.display -> clear();

}

//////////////////////////////////////////////////////////////////

/////////////////////////////// OTA ///////////////////////////////////////
void notFoundResponse(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

// handle the upload of the firmware
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    // handle upload and update
    if (!index)
    {
        Serial.printf("Update: %s\n", filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        { //start with max available size
            Update.printError(Serial);
        }
    }

    /* flashing firmware to ESP*/
    if (len)
    {
        Update.write(data, len);
    }

    if (final)
    {
        if (Update.end(true))
        { //true to set the size to the current progress
            Serial.printf("Update Success: %ub written\nRebooting...\n", index+len);
        }
        else
        {
            Update.printError(Serial);
        }
    }
    // alternative approach
    // https://github.com/me-no-dev/ESPAsyncWebServer/issues/542#issuecomment-508489206
}
///////////////////////////////////////////////////////////////////////

void setup() {

  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  
  
//////////////////////////// wifi stuff /////////////////////////////////
  Serial.begin(115200);  
  ///////////////////Start WiFi ///////////////////////////////////////
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server, &dns);
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,176), IPAddress(192,168,1,1), IPAddress(255,255,255,0), IPAddress(192,168,1,1), IPAddress(192,168,1,1));
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("PH")) {
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
  //WebSerial.print("local ip is: ");
  //WebSerial.println(WiFi.localIP());

  //server.on("/", handleRoot);
  //server.on("/test.svg", drawGraph);
/*  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });*/

    /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNSesponder started");

  //pinMode(34,INPUT);
  
  server.on("/ph", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPh().c_str());
  });
    server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });

//////////////////////// OTA Web ///////////////////////////////////

    // web interface for OTA
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", indexPage);
        response->addHeader("Connection", "close");
        request->send(response);
    });
    server.on("/firmware", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "text/html", firmwarePage);
        response->addHeader("Connection", "close");
        request->send(response);
    });
    // handling uploading firmware file
    server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!Update.hasError()) {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "OK");
            response->addHeader("Connection", "close");
            request->send(response);
            ESP.restart();
        } else {
            AsyncWebServerResponse *response = request->beginResponse(500, "text/plain", "ERROR");
            response->addHeader("Connection", "close");
            request->send(response);
        } }, handleUpload);
    server.onNotFound(notFoundResponse);


///////////////////////////////////////////////////////////////////////
  server.begin();
  ph.begin();
  sensors.begin();

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);

  logo();
    if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("LOST WiFi restarting");
    //atoAwcUtil->webSerialWriteLine("LOST WiFi restarting");
    ESP.restart();
  }

  //delay(300);
  Heltec.display->clear();
  
  //WiFi.disconnect(true);//重新初始化WIFI
//  delay(1000);
//  WiFi.mode(WIFI_STA);
//  WiFi.setAutoConnect(true);


  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();  
  sensors.requestTemperatures(); 
  temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureF);
  Serial.println("ºF");
//loop to blink without delay
  unsigned long currentMillis = millis();
  if ( (currentMillis - previousMillis >= interval)&&(WiFi.status() == WL_CONNECTED)) {
  // save the last time you blinked the LED
  previousMillis = currentMillis;
  // if the LED is off turn it on and vice-versa:
  ledState = not(ledState);
  // set the LED with the ledState of the variable:
  digitalWrite(LED,  ledState);
  }
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
    Serial.print("voltage:");
    Serial.println(voltage, 4);
    
    temperature = readTemp().toFloat();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue, 4);
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
  //delay(300);

  displaySensor();
}



//////////////////////////////////////////////////////////////
// call back needed for wifi
/////////////////////////////////////////////////////////////
void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  //myWiFiManager->startConfigPortal("ATOAWC");
  //myWiFiManager->autoConnect("DOSER");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());

}
