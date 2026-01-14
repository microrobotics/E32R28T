//Test E32R28T Display with touch
//Calibrate Display 
//Clear Button
//Recalibrate Button 

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define TP_CS 33
#define TP_IRQ 36

TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TP_CS, TP_IRQ);

// Drawing variables
int lastX = -1;
int lastY = -1;
bool buttonPressed = false;

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
    
    // Draw CLEAR button at top
    tft.fillRect(10, 5, 80, 30, TFT_RED);
    tft.drawRect(10, 5, 80, 30, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 12);
    tft.println("CLEAR");
    
    // Draw RECAL button next to CLEAR
    tft.fillRect(100, 5, 80, 30, TFT_BLUE);
    tft.drawRect(100, 5, 80, 30, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(110, 12);
    tft.println("RECAL");
    
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(1);
    tft.setCursor(190, 15);
    tft.println("Draw!");
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
    // Drawing mode
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      int x = map(p.x, minX, maxX, 0, tft.width());
      int y = map(p.y, minY, maxY, 0, tft.height());
      
      x = constrain(x, 0, tft.width() - 1);
      y = constrain(y, 0, tft.height() - 1);
      
      // Check if CLEAR button is pressed
      if (y >= 0 && y <= 40 && x >= 5 && x <= 95 && !buttonPressed) {
        // Button press visual feedback
        tft.fillRect(10, 5, 80, 30, TFT_DARKGREY);
        tft.drawRect(10, 5, 80, 30, TFT_WHITE);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(20, 12);
        tft.println("CLEAR");
        delay(100);
        
        // Clear screen and redraw buttons
        tft.fillScreen(TFT_BLACK);
        tft.fillRect(10, 5, 80, 30, TFT_RED);
        tft.drawRect(10, 5, 80, 30, TFT_WHITE);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(20, 12);
        tft.println("CLEAR");
        
        tft.fillRect(100, 5, 80, 30, TFT_BLUE);
        tft.drawRect(100, 5, 80, 30, TFT_WHITE);
        tft.setCursor(110, 12);
        tft.println("RECAL");
        
        tft.setTextColor(TFT_CYAN);
        tft.setTextSize(1);
        tft.setCursor(190, 15);
        tft.println("Draw!");
        
        // Reset drawing
        lastX = -1;
        lastY = -1;
        buttonPressed = true;
        delay(200);
      }
      // Check if RECAL button is pressed
      else if (y >= 0 && y <= 40 && x >= 95 && x <= 185 && !buttonPressed) {
        // Button press visual feedback
        tft.fillRect(100, 5, 80, 30, TFT_DARKGREY);
        tft.drawRect(100, 5, 80, 30, TFT_WHITE);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(110, 12);
        tft.println("RECAL");
        delay(100);
        
        // Restart calibration
        calibrated = false;
        calStep = 0;
        lastX = -1;
        lastY = -1;
        buttonPressed = true;
        showCalibrationScreen();
        delay(200);
      }
      else if (y > 45) {  // Only draw below the button area
        buttonPressed = false;
        // Draw line from last position to current position
        if (lastX != -1 && lastY != -1 && lastY > 45) {
          tft.drawLine(lastX, lastY, x, y, TFT_CYAN);
        } else {
          // First touch - draw a dot
          tft.fillCircle(x, y, 2, TFT_CYAN);
        }
        
        // Update last position
        lastX = x;
        lastY = y;
      }
      
      delay(5);
    } else {
      // Reset last position when not touching (lift pen)
      lastX = -1;
      lastY = -1;
      buttonPressed = false;
    }
    
    delay(5);
  }
}
