#include "oled.h"

  Oled::Oled(){
  }

  void Oled::beginOled(){
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/); 
    //Heltec.display->flipScreenVertically();
    //Heltec.display->setFont(ArialMT_Plain_10);
   }

void Oled::drawbitmap(uint8_t posX, uint8_t posY, const uint8_t *bitmap, uint8_t w, uint8_t h) {
      Heltec.display->clear();
      //Heltec.display->display();
   // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    Heltec.display->drawXbm(posX, posY, WiFi_Logo_width, WiFi_Logo_height, bitmap);
   Heltec.display->display();
 }

  void Oled::drawText(String alignment, int fontSize, int rotation, String txt){
    if(alignment = "center"){
      Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    }
      Heltec.display->clear();
      Heltec.display->display();
      if(rotation = 0){
        Heltec.display->screenRotate(ANGLE_0_DEGREE);
      }
       if(fontSize == 10){
         Heltec.display->setFont(ArialMT_Plain_10);
      }else if(fontSize == 16){
         Heltec.display->setFont(ArialMT_Plain_16);
      }else if(fontSize == 24){
         Heltec.display->setFont(ArialMT_Plain_24);
      }
        Heltec.display->drawString(64, 32-16/2, txt);
      Heltec.display->display();
  }
