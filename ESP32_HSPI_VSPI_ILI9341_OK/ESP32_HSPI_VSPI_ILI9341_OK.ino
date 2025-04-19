/*
 * WARNING: DOES NOT WORK WITH ESPRESSIF ESP32 CORE 1.05 OR 1.06 ... TESTED WITH 1.04 which enumerates correctly
file: ESP32_HSPI_ILI9341_OK.ino
revised: 09/19/20, (orig 05/09/19).

Compiled w/ Arduino 1.8.13 under Linux on 20200919 mrb Test verified to run from 80MHz to 240MHz)
  Sketch uses 234897 bytes (17%) of program storage space. Maximum is 1310720 bytes.
  Global variables use 15732 bytes (4%) of dynamic memory, leaving 311948 bytes for local variables. Maximum is 327680 bytes.
---------------------------------------------
NOTES:
  Adafruit graphic libraries and ILI9341 driver
    included as sketch-encapsulated for demostration purposes
  Written by Limor Fried/Ladyada for Adafruit Industries.
  Minor changes & Adafruit Library encapsulation by M. Ray Burneette 12/01/2019
  Tested at 240MHz successfully
*******************************************************/

#include "SPI.h"
#include "./Adafruit_GFX.h"F
#include "./Adafruit_ILI9341.h"

const char *progname = "\n\n ILI9341 GfxTest...(20200418)";
const char *xxxx = "- requires Adafruit_ILI9341_050519";

#define USE_HSPI_PORT                                 // optional it seems

// Software Pin Name  // GPIO: HSPI    VSPI           //  ILI9341 J2      // Notes: all HSPI pins can remap
// -------------------------------------------------- //    1 VDD 3.3V
// -------------------------------------------------- //    2 GND
#define HSP_CS                  15                    //    3 CS          <========== xSPI centric
#define VSP_CS                           5
#define HSP_RST                 27                    //    4 RST         <========== User Defined (Tie to VDD)
#define VSP_RST                         25
#define HSP_DC                  26                    //    5 D/C         <========== User Defined
#define VSP_DC                          16
#define HSP_MOSI                13                    //    6 SDI-MOSI    <========== xSPI centric
#define VSP_MOSI                        23      
#define HSP_SCLK                14                    //    7 SCK         <========== xSPI centric
#define VSP_SCLK                        18
// -------------------------------------------------- //    8 LED                     !!! 3.3V NOT 5V !!!
#define HSP_MISO                12                    //    9 SDO-MISO    <========== xSPI centric (Not used ILI9341)
#define VSP_MISO                        19

// VSPI (default under Arduino)
SPIClass  SPI1(VSPI);
// Use ESP32 hardware VSPI and above defines
//Adafruit_ILI9341 tft0=Adafruit_ILI9341(VSP_CS,VSP_DC,VSP_RST);
Adafruit_ILI9341 tft0 = Adafruit_ILI9341(&SPI1, VSP_DC, VSP_CS, VSP_RST);

// HSPI 
SPIClass  SPI2(HSPI);
// static const int spiClk = 1000000;           // 1 MHz ... default is 40MHz
// Use ESP32 hardware HSPI and above defines
Adafruit_ILI9341 tft1 = Adafruit_ILI9341(&SPI2, HSP_DC, HSP_CS, HSP_RST);


void setup(void) 
{
  pinMode(VSP_CS, OUTPUT); //VSPI SS (may not be required for 'default')
  pinMode(HSP_CS, OUTPUT); //HSPI SS
  Serial.begin(115200);
  Serial.println(progname); 
  Serial.println(xxxx);
  tft0.begin();
  tft1.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x0 = tft0.readcommand8(ILI9341_RDMODE);
  uint8_t x1 = tft1.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode 0: 0x"); Serial.println(x0, HEX);
  Serial.print("Display Power Mode 1: 0x"); Serial.println(x1, HEX);
  x0 = tft0.readcommand8(ILI9341_RDMADCTL);
  x1 = tft1.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode 0: 0x"); Serial.println(x0, HEX);
  Serial.print("MADCTL Mode 1: 0x"); Serial.println(x1, HEX);
  x0 = tft0.readcommand8(ILI9341_RDPIXFMT);
  x1 = tft1.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format 0: 0x"); Serial.println(x0, HEX);
  Serial.print("Pixel Format 1: 0x"); Serial.println(x1, HEX);
  x0 = tft0.readcommand8(ILI9341_RDIMGFMT);
  x1 = tft1.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format 0: 0x"); Serial.println(x0, HEX);
  Serial.print("Image Format 1: 0x"); Serial.println(x1, HEX);
  x0 = tft0.readcommand8(ILI9341_RDSELFDIAG);
  x1 = tft1.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic 0: 0x"); Serial.println(x0, HEX);
  Serial.print("Self Diagnostic 1: 0x"); Serial.println(x1, HEX);
}

void loop(void) 
{
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft0.setRotation(rotation);
    testText0();
    delay(1000);
  }
  gfxtest0();

  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft1.setRotation(rotation);
    testText1();
    delay(1000);
  }
  gfxtest1();
}
