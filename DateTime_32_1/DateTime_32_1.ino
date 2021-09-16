#include "datetime.h"
#include <WiFi.h>
#include "time.h"

Datetime* _datetime;

const char* ssid     = "RoodRouter";
const char* password = "";


void setup() {
    Serial.begin(115200);

  // Connect to Wi-Fi
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
  // put your setup code here, to run once:
  _datetime = new Datetime(-21600,true);

}

void loop() {
  // put your main code here, to run repeatedly:
  _datetime->updateTime();
  Serial.print("Year is : ");
  Serial.println(_datetime->getCurrentYear());
  Serial.print("Month is : ");
  Serial.println(_datetime->getCurrentMonth());
  Serial.println(_datetime->getCurrentMonthName());  
  Serial.print("Day is : ");
  Serial.println(_datetime->getCurrentDay());
  Serial.print("Hour is : ");
  Serial.println(_datetime->getCurrentHour());
  Serial.print("Second is is : ");
  Serial.println(_datetime->getCurrentSecond());
  Serial.print("Day of week is : ");
  Serial.println(_datetime->getWeekDay());

  delay(1000);

}
