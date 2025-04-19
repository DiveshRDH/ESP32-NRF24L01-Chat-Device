
// Software Pin Name  // GPIO: HSPI    VSPI           //  ILI9341 J2      // Notes: all HSPI pins can remap
// -------------------------------------------------- //    1 VDD 3.3V
// -------------------------------------------------- //    2 GND
#define HSP_CS                  15                    //    3 CS          <========== xSPI centric
#define VSP_CS                           5
#define HSP_RST    -1   //      27                    //    4 RST         <========== User Defined (Tie to VDD)
#define VSP_RST    -1   //              25
#define HSP_DC                  26                    //    5 D/C         <========== User Defined
#define VSP_DC                          16
#define HSP_MOSI                13                    //    6 SDI-MOSI    <========== xSPI centric
#define VSP_MOSI                        23      
#define HSP_SCLK                14                    //    7 SCK         <========== xSPI centric
#define VSP_SCLK                        18
// -------------------------------------------------- //    8 LED                     !!! 3.3V NOT 5V !!!
#define HSP_MISO    -1   //     12                    //    9 SDO-MISO    <========== xSPI centric (Not used ILI9341)
#define VSP_MISO    -1   //             19


// VSPI (default under Arduino)
SPIClass  SPI1(VSPI);
// Use ESP32 hardware VSPI and above defines
//Adafruit_ILI9341 tft0=Adafruit_ILI9341(VSP_CS,VSP_DC,VSP_RST);
Adafruit_ILI9341 tft0=Adafruit_ILI9341(&SPI1, VSP_DC, VSP_CS, VSP_RST);

// HSPI 
SPIClass  SPI2(HSPI);
// static const int spiClk = 1000000;           // 1 MHz ... default is 40MHz
// Use ESP32 hardware HSPI and above defines
Adafruit_ILI9341 tft1=Adafruit_ILI9341(&SPI2, HSP_DC, HSP_CS, HSP_RST);

============================================================================================================

// pins_Arduino.h
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16

#define analogInputToDigitalPin(p)  (((p)<20)?(esp32_adc2gpio[(p)]):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)

static const uint8_t LED_BUILTIN = 5;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility



static const uint8_t TX = 1;
static const uint8_t RX = 3;

static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 18;

static const uint8_t A0 = 36;
static const uint8_t A3 = 39;
static const uint8_t A4 = 32;
static const uint8_t A5 = 33;
static const uint8_t A6 = 34;
static const uint8_t A7 = 35;
static const uint8_t A10 = 4;
static const uint8_t A11 = 0;
static const uint8_t A12 = 2;
static const uint8_t A13 = 15;
static const uint8_t A14 = 13;
static const uint8_t A15 = 12;
static const uint8_t A16 = 14;
static const uint8_t A17 = 27;
static const uint8_t A18 = 25;
static const uint8_t A19 = 26;

static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;

static const uint8_t DAC1 = 25;
static const uint8_t DAC2 = 26;

#endif /* Pins_Arduino_h */

// ===================================================================================================================================
For the history ... as of the date of this post, the pins in SPI have been renamed to be:

int mosi_io_num – GPIO for MOSI or -1 if not used.
int miso_io_num – GPIO for MISO or -1 if not used.
int sclk_io_num – GPIO pin for CLK or -1 if not used.
int quadwp_io_num – Specify -1 if not used.
int quadhd_io_num – Specify -1 if not used.
Free book on ESP32 available here: https://leanpub.com/kolban-ESP32
// ===================================================================================================================================

// linker shows no dependencies for Arduino/libraries/Adafruit*
Sketch uses 234897 bytes (17%) of program storage space. Maximum is 1310720 bytes.
Global variables use 15732 bytes (4%) of dynamic memory, leaving 311948 bytes for local variables. Maximum is 327680 bytes.
python /home/ray/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 /home/ray/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/boot_app0.bin 0x1000 /home/ray/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/sdk/bin/bootloader_qio_80m.bin 0x10000 /tmp/arduino_build_662290/ESP32_HSPI_VSPI_ILI9341_OK.ino.bin 0x8000 /tmp/arduino_build_662290/ESP32_HSPI_VSPI_ILI9341_OK.ino.partitions.bin 
esptool.py v2.6
Serial port /dev/ttyUSB0
Connecting.....
Chip is ESP32D0WDQ6 (revision 0)
Features: WiFi, BT, Dual Core, Coding Scheme None
MAC: 24:0a:c4:05:78:6c
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 921600
Changed.
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 8192 bytes to 47...
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.0 seconds (effective 36287.5 kbit/s)...
Hash of data verified.
Compressed 17392 bytes to 11186...
Wrote 17392 bytes (11186 compressed) at 0x00001000 in 0.2 seconds (effective 826.6 kbit/s)...
Hash of data verified.
Compressed 235008 bytes to 121456...
Wrote 235008 bytes (121456 compressed) at 0x00010000 in 2.2 seconds (effective 864.0 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 128...
Wrote 3072 bytes (128 compressed) at 0x00008000 in 0.0 seconds (effective 7571.0 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
// =========================================================== Sample Serial Output ==================================================

ets Jun  8 2016 00:22:57

rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0018,len:4
load:0x3fff001c,len:1216
ho 0 tail 12 room 4
load:0x40078000,len:9720
ho 0 tail 12 room 4
load:0x40080400,len:6352
entry 0x400806b8


 ILI9341 GfxTest...(20200418)
- requires Adafruit_ILI9341_050519
Display Power Mode 0: 0x0
Display Power Mode 1: 0x0
MADCTL Mode 0: 0x0
MADCTL Mode 1: 0x0
Pixel Format 0: 0x0
Pixel Format 1: 0x0
Image Format 0: 0x0
Image Format 1: 0x0
Self Diagnostic 0: 0x0
Self Diagnostic 1: 0x0
Benchmark Disp:0         Time (microseconds)
Screen fill              195275
Text                      27713
Lines                    268193
Horiz/Vert Lines          17597
Rectangles (outline)      11624
Rectangles (filled)      405621
Circles (filled)          76616
Circles (outline)        118038
Triangles (outline)       58792
Triangles (filled)       154267
Rounded rects (outline)   42664
Rounded rects (filled)   412482
Done!


Benchmark Disp:1         Time (microseconds)
Screen fill              195241
Text                      27634
Lines                    268200
Horiz/Vert Lines          17597
Rectangles (outline)      11611
Rectangles (filled)      405628
Circles (filled)          76578
Circles (outline)        118022
Triangles (outline)       58774
Triangles (filled)       154244
Rounded rects (outline)   42625
Rounded rects (filled)   412482
Done!
