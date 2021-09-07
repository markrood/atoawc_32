#ifndef OLED_32_h
#define OLED_32_h

#include "Arduino.h"
#include "heltec.h"

#define WiFi_Logo_width 60
#define WiFi_Logo_height 36

#pragma once

class Oled
{
  public:
    Oled();
    void beginOled();
    void drawbitmap(uint8_t posX, uint8_t posY, const uint8_t *bitmap, uint8_t w, uint8_t h);
    void drawText(String alignment, int fontSize, int rotation, String txt);
};

#endif
