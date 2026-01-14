#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define TP_CS 33
#define TP_IRQ 36

TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TP_CS, TP_IRQ);

void setup() {
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Touch Test with IRQ");
  
  touchSPI.begin(25, 39, 32, TP_CS);
  ts.begin(touchSPI);
}

void loop() {
  int irq = digitalRead(TP_IRQ);
  
  // Update IRQ status
  tft.fillRect(10, 40, 150, 20, TFT_BLACK);
  tft.setCursor(10, 40);
  tft.setTextColor(irq == LOW ? TFT_GREEN : TFT_RED);
  tft.print("IRQ: ");
  tft.println(irq == LOW ? "LOW" : "HIGH");
  
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    int x = map(p.x, 300, 3800, 0, tft.width());
    int y = map(p.y, 400, 3800, 0, tft.height());
    
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
