// This is a basic Hello World Example 
// Display E32R28T 2.8" Passice Touch Display 
// Filename 00_E32R28T_HelloWorld.ino


#include <TFT_eSPI.h>

#define TFT_BL 21   // Backlight control pin (E32R28T)

TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Backlight setup
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);   // Turn backlight ON

  // Display init
  tft.init();
  tft.setRotation(1);           // Landscape
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 60);
  tft.println("Hello World!");
}

void loop() {
  // Nothing needed
}
