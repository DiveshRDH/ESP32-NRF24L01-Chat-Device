/********************************************************/
void gfxtest0( void )
{

  Serial.println(F("Benchmark Disp:0         Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen0());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText0());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines0(ILI9341_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines0(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects0(ILI9341_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects0(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles0(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles0(10, ILI9341_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles0());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles0());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects0());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects0());
  delay(500);

  Serial.println(F("Done!\n\r"));
}


void gfxtest1( void )
{

  Serial.println(F("Benchmark Disp:1         Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen1());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText1());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines1(ILI9341_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines1(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects1(ILI9341_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects1(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles1(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles1(10, ILI9341_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles1());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles1());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects1());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects1());
  delay(500);

  Serial.println(F("Done!"));
}


unsigned long testFillScreen0() 
{
  unsigned long start = micros();
  tft0.fillScreen(ILI9341_BLACK);
  yield();
  tft0.fillScreen(ILI9341_RED);
  yield();
  tft0.fillScreen(ILI9341_GREEN);
  yield();
  tft0.fillScreen(ILI9341_BLUE);
  yield();
  tft0.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testFillScreen1() 
{
  unsigned long start = micros();
  tft1.fillScreen(ILI9341_BLACK);
  yield();
  tft1.fillScreen(ILI9341_RED);
  yield();
  tft1.fillScreen(ILI9341_GREEN);
  yield();
  tft1.fillScreen(ILI9341_BLUE);
  yield();
  tft1.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}


unsigned long testText0() {
  tft0.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft0.setCursor(0, 0);
  tft0.setTextColor(ILI9341_WHITE);  tft0.setTextSize(1);
  tft0.println("Hello World!");
  tft0.setTextColor(ILI9341_YELLOW); tft0.setTextSize(2);
  tft0.println(1234.56);
  tft0.setTextColor(ILI9341_RED);    tft0.setTextSize(3);
  tft0.println(0xDEADBEEF, HEX);
  tft0.println();
  tft0.setTextColor(ILI9341_GREEN);
  tft0.setTextSize(5);
  tft0.println("Groop");
  tft0.setTextSize(2);
  tft0.println("I implore thee,");
  tft0.setTextSize(1);
  tft0.println("my foonting turlingdromes.");
  tft0.println("And hooptiously drangle me");
  tft0.println("with crinkly bindlewurdles,");
  tft0.println("Or I will rend thee");
  tft0.println("in the gobberwarts");
  tft0.println("with my blurglecruncheon,");
  tft0.println("see if I don't!");
  return micros() - start;
}

unsigned long testText1() {
  tft1.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft1.setCursor(0, 0);
  tft1.setTextColor(ILI9341_WHITE);  tft1.setTextSize(1);
  tft1.println("Hello World!");
  tft1.setTextColor(ILI9341_YELLOW); tft1.setTextSize(2);
  tft1.println(1234.56);
  tft1.setTextColor(ILI9341_RED);    tft1.setTextSize(3);
  tft1.println(0xDEADBEEF, HEX);
  tft1.println();
  tft1.setTextColor(ILI9341_GREEN);
  tft1.setTextSize(5);
  tft1.println("Groop");
  tft1.setTextSize(2);
  tft1.println("I implore thee,");
  tft1.setTextSize(1);
  tft1.println("my foonting turlingdromes.");
  tft1.println("And hooptiously drangle me");
  tft1.println("with crinkly bindlewurdles,");
  tft1.println("Or I will rend thee");
  tft1.println("in the gobberwarts");
  tft1.println("with my blurglecruncheon,");
  tft1.println("see if I don't!");
  return micros() - start;
}


unsigned long testLines0(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft1.width(),
                h = tft1.height();

  tft0.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft0.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft0.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft0.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft0.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft0.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}


unsigned long testLines1(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft1.width(),
                h = tft1.height();

  tft1.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft1.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft1.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft1.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft1.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft1.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}


unsigned long testFastLines0(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft0.width(), h = tft1.height();

  tft0.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft0.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft0.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testFastLines1(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft1.width(), h = tft1.height();

  tft1.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft1.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft1.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}


unsigned long testRects0(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft0.width()  / 2,
                cy = tft0.height() / 2;

  tft0.fillScreen(ILI9341_BLACK);
  n     = min(tft0.width(), tft0.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft0.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}


unsigned long testRects1(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft1.width()  / 2,
                cy = tft1.height() / 2;

  tft1.fillScreen(ILI9341_BLACK);
  n     = min(tft1.width(), tft1.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft1.drawRect(cx-i2, cy-i2, i, i, color);
  }
  return micros() - start;
}


unsigned long testFilledRects0(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft0.width()  / 2 - 1,
                cy = tft0.height() / 2 - 1;

  tft0.fillScreen(ILI9341_BLACK);
  n = min(tft0.width(), tft0.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft0.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft0.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }
  return t;
}


unsigned long testFilledRects1(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft1.width()  / 2 - 1,
                cy = tft1.height() / 2 - 1;

  tft1.fillScreen(ILI9341_BLACK);
  n = min(tft1.width(), tft1.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft1.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft1.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }
  return t;
}


unsigned long testFilledCircles0(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft0.width(), h = tft0.height(), r2 = radius * 2;

  tft0.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft0.fillCircle(x, y, radius, color);
    }
  }
  return micros() - start;
}


unsigned long testFilledCircles1(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft1.width(), h = tft1.height(), r2 = radius * 2;

  tft1.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft1.fillCircle(x, y, radius, color);
    }
  }
  return micros() - start;
}


unsigned long testCircles0(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft0.width()  + radius,
                h = tft0.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft0.drawCircle(x, y, radius, color);
    }
  }
  return micros() - start;
}


unsigned long testCircles1(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft1.width()  + radius,
                h = tft1.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft1.drawCircle(x, y, radius, color);
    }
  }
  return micros() - start;
}


unsigned long testTriangles0() {
  unsigned long start;
  int           n, i, cx = tft0.width()  / 2 - 1,
                      cy = tft0.height() / 2 - 1;

  tft0.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft0.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft0.color565(i, i, i));
  }
  return micros() - start;
}


unsigned long testTriangles1() {
  unsigned long start;
  int           n, i, cx = tft1.width()  / 2 - 1,
                      cy = tft1.height() / 2 - 1;

  tft1.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft1.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft1.color565(i, i, i));
  }
  return micros() - start;
}


unsigned long testFilledTriangles0() {
  unsigned long start, t = 0;
  int           i, cx = tft0.width()  / 2 - 1,
                   cy = tft0.height() / 2 - 1;

  tft0.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft0.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft1.color565(0, i*10, i*10));
    t += micros() - start;
    tft0.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft0.color565(i*10, i*10, 0));
    yield();
  }
  return t;
}


unsigned long testFilledTriangles1() {
  unsigned long start, t = 0;
  int           i, cx = tft1.width()  / 2 - 1,
                   cy = tft1.height() / 2 - 1;

  tft1.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft1.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft1.color565(0, i*10, i*10));
    t += micros() - start;
    tft1.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft1.color565(i*10, i*10, 0));
    yield();
  }
  return t;
}


unsigned long testRoundRects0() {
  unsigned long start;
  int           w, i, i2,
                cx = tft0.width()  / 2 - 1,
                cy = tft0.height() / 2 - 1;

  tft0.fillScreen(ILI9341_BLACK);
  w     = min(tft0.width(), tft1.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft0.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft0.color565(i, 0, 0));
  }
  return micros() - start;
}


unsigned long testRoundRects1() {
  unsigned long start;
  int           w, i, i2,
                cx = tft1.width()  / 2 - 1,
                cy = tft1.height() / 2 - 1;

  tft1.fillScreen(ILI9341_BLACK);
  w     = min(tft1.width(), tft1.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft1.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft1.color565(i, 0, 0));
  }
  return micros() - start;
}


unsigned long testFilledRoundRects0() {
  unsigned long start;
  int           i, i2,
                cx = tft0.width()  / 2 - 1,
                cy = tft0.height() / 2 - 1;

  tft0.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft0.width(), tft0.height()); i>20; i-=6) {
    i2 = i / 2;
    tft0.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft0.color565(0, i, 0));
    yield();
  }
  return micros() - start;
}


unsigned long testFilledRoundRects1() {
  unsigned long start;
  int           i, i2,
                cx = tft1.width()  / 2 - 1,
                cy = tft1.height() / 2 - 1;

  tft1.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft1.width(), tft1.height()); i>20; i-=6) {
    i2 = i / 2;
    tft1.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft1.color565(0, i, 0));
    yield();
  }
  return micros() - start;
}
