#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// TFT display pins
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

// Create TFT object using hardware SPI (ESP32 default: MOSI=23, SCK=18)
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Starting ILI9341...");

  tft.begin();
  tft.setRotation(1); // Landscape
  tft.fillScreen(ILI9341_BLACK);

  // Display startup messages
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(10, 20);
  tft.println("ESP32 + ILI9341");

  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(10, 60);
  tft.println("Hardware SPI OK");

  delay(1000);
}

void loop() {
  static int count = 0;

  // Clear just the count area
  tft.fillRect(10, 100, 200, 30, ILI9341_BLACK);

  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(3);
  tft.setCursor(10, 100);
  tft.printf("Count: %d", count++);

  delay(1000);
}
