#include <TFT_eSPI.h>

// ==================== Pin Definitions from E32R28T ====================
// LCD Display Pins (Custom SPI)
// The E32R28T display does not use standard SPI pin assignments.
// The pins can either be updated in the TFT_eSPI library User_Setup.h or 
// defined here to override the User_Setup.h settings.

#define TFT_MISO  12
#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   -1    // Connected to EN which is master reset pin on the ESP32
#define TFT_BL    21

// Setting TFT_RST to -1 tells the TFT_eSPI library:
// Don't use a dedicated GPIO pin for reset
// The reset is handled elsewhere


TFT_eSPI tft = TFT_eSPI();

void setup() {
  
  // Configure backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  // Turn backlight ON
  
  
  // Initialize display
  tft.init();
  tft.setRotation(1);  // Landscape mode
  
  // Clear screen
  tft.fillScreen(TFT_BLACK);
  
  // Display "Hello World" text
  tft.setTextSize(3);
  tft.setCursor(40, 100);
  tft.println("Hello World!");
  
}

void loop() {
  // Nothing to do here
}
