// This example performs initial startup calibration of the XPT2046 resistive 
// touch controller to ensure accurate touch coordinate mapping.

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define TP_CS 33
#define TP_IRQ 36

TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TP_CS, TP_IRQ);

// Calibration values
int minX = 300, maxX = 3800;
int minY = 400, maxY = 3800;

bool calibrated = false;
int calStep = 0;
int rawMinX, rawMaxX, rawMinY, rawMaxY;

void drawCrosshair(int x, int y) {
  tft.drawCircle(x, y, 10, TFT_RED);
  tft.drawCircle(x, y, 5, TFT_RED);
  tft.drawLine(x - 15, y, x + 15, y, TFT_RED);
  tft.drawLine(x, y - 15, x, y + 15, TFT_RED);
}

void showCalibrationScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  
  if (calStep == 0) {
    tft.setCursor(40, 100);
    tft.println("Touch TOP-LEFT");
    tft.setCursor(80, 130);
    tft.println("corner");
    drawCrosshair(20, 20);
  }
  else if (calStep == 1) {
    tft.setCursor(20, 100);
    tft.println("Touch BOTTOM-RIGHT");
    tft.setCursor(80, 130);
    tft.println("corner");
    drawCrosshair(tft.width() - 20, tft.height() - 20);
  }
  else if (calStep == 2) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(60, 100);
    tft.setTextColor(TFT_GREEN);
    tft.println("Calibrated!");
    delay(1000);
    
    // Calculate calibration values
    minX = rawMinX;
    maxX = rawMaxX;
    minY = rawMinY;
    maxY = rawMaxY;
    
    calibrated = true;
    
    // Start main screen
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Touch screen...");
  }
}

void setup() {
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  touchSPI.begin(25, 39, 32, TP_CS);
  ts.begin(touchSPI);
  ts.setRotation(3);
  
  // Start calibration
  showCalibrationScreen();
}

void loop() {
  if (!calibrated) {
    // Calibration mode
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      
      if (calStep == 0) {
        // Top-left corner
        rawMinX = p.x;
        rawMinY = p.y;
        calStep++;
        delay(500);
        showCalibrationScreen();
      }
      else if (calStep == 1) {
        // Bottom-right corner
        rawMaxX = p.x;
        rawMaxY = p.y;
        calStep++;
        delay(500);
        showCalibrationScreen();
      }
    }
  }
  else {
    // Normal mode
    int irq = digitalRead(TP_IRQ);
    
    // Update IRQ status
    tft.fillRect(10, 40, 150, 20, TFT_BLACK);
    tft.setCursor(10, 40);
    tft.setTextColor(irq == LOW ? TFT_GREEN : TFT_RED);
    tft.print("IRQ: ");
    tft.println(irq == LOW ? "LOW" : "HIGH");
    
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      int x = map(p.x, minX, maxX, 0, tft.width());
      int y = map(p.y, minY, maxY, 0, tft.height());
      
      x = constrain(x, 0, tft.width() - 1);
      y = constrain(y, 0, tft.height() - 1);
      
      tft.fillRect(10, 70, 200, 50, TFT_BLACK);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(10, 70);
      tft.print("X: ");
      tft.println(x);
      tft.setCursor(10, 95);
      tft.print("Y: ");
      tft.println(y);
      
      delay(100);
    }
    
    delay(50);
  }
}
