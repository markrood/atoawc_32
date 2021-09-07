
#ifndef OLED_32_h
#define OLED_32_h


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define NUMFLAKES     10 // Number of snowflakes in the animation example

//#define LOGO_HEIGHT   16
//#define LOGO_WIDTH    16

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

class Oled
{
  public:
    Oled();
    void beginOled();
    
    void drawline();      // Draw many lines

    void drawrect();      // Draw rectangles (outlines)
  
    void fillrect();      // Draw rectangles (filled)
  
    void drawcircle();    // Draw circles (outlines)
  
    void fillcircle();    // Draw circles (filled)
  
    void drawroundrect(); // Draw rounded rectangles (outlines)
  
    void fillroundrect(); // Draw rounded rectangles (filled)
  
    void drawtriangle();  // Draw triangles (outlines)
  
    void filltriangle();  // Draw triangles (filled)
  
    void drawchar();      // Draw characters of the default font
  
    void drawstyles();    // Draw 'stylized' characters
  
    void scrolltext();    // Draw scrolling text
  
    void drawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);    // Draw a small bitmap image

    void animate(const uint8_t *bitmap, uint8_t w, uint8_t h);

    void oledDispaly();
    void clearDisplay();
    void drawPixel(int p1, int p2, const char *txt);
    void invertDisplay(bool val);

  private:
    Adafruit_SSD1306 display;


};

#endif
