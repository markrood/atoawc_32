#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "oled.h"

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

const char white[] = "WHITE";

      static const unsigned char PROGMEM logo_bmp[] =
    { B00000000, B11000000,
      B00000001, B11000000,
      B00000001, B11000000,
      B00000011, B11100000,
      B11110011, B11100000,
      B11111110, B11111000,
      B01111110, B11111111,
      B00110011, B10011111,
      B00011111, B11111100,
      B00001101, B01110000,
      B00011011, B10100000,
      B00111111, B11100000,
      B00111111, B11110000,
      B01111100, B11110000,
      B01110000, B01110000,
      B00000000, B00110000 };


Oled *oled;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  oled = new Oled();
  oled->beginOled();
    

}

void loop() {
  // put your main code here, to run repeatedly:

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  oled->oledDispaly();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  oled->clearDisplay();

  // Draw a single pixel in white
  oled->drawPixel(10, 10, white);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  oled->oledDispaly();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
 // Invert and restore display, pausing in-between
  oled->invertDisplay(true);
  delay(1000);
  oled->invertDisplay(false);
  delay(1000);

  oled->drawline();
  oled->drawrect();
  oled->fillrect();
  oled->drawcircle();
  oled->fillcircle();
  oled->drawroundrect();
  oled->fillroundrect();
  oled->drawtriangle();
  oled->filltriangle();
  oled->drawchar();
  oled->drawstyles();
  oled->scrolltext();
  oled->drawbitmap(logo_bmp,LOGO_WIDTH, LOGO_HEIGHT);

   // Invert and restore display, pausing in-between
  oled->invertDisplay(true);
  delay(1000);
  oled->invertDisplay(false);
  delay(1000);

  oled->animate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps

}
