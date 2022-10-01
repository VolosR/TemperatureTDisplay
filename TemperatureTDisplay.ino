#include "TFT_eSPI.h"
#include "image.h"
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite sprTemperature = TFT_eSprite(&tft);  //sprite for temperature


int iW = 480;
int iH = 244;

int w = 320;
int h = 170;
int m = w;

int xt = 8;
int yt = 8;

int start = 1;
unsigned short imageS[54400] = { 0 };
int pos = 0;
int x = 0;
int y = 30;
int changeX = 1;
int changeY = 1;

String temperature;
long pressure;

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  sprite.createSprite(320, 170);
  sprite.setSwapBytes(true);

  sprTemperature.createSprite(86, 160);
  sprTemperature.fillSprite(TFT_GREEN);
  sprTemperature.setTextDatum(4);
  sprTemperature.setTextColor(TFT_WHITE, TFT_GREEN);
  sprTemperature.setFreeFont(&Orbitron_Light_24);

  Wire.begin(43, 44);
  bmp.begin();
}

void loop() {
  sprTemperature.fillSprite(TFT_BLACK);
  sprTemperature.setFreeFont(&Orbitron_Light_24);
  pos = x + iW * y;
  start = pos;
  m = w + pos;
  for (int i = 0; i < 54400; i++) {
    if (start % m == 0) {
      start += iW - w;
      m += iW;
    }
    imageS[i] = picture[start];
    start++;
  }
  x += changeX;
  if (x == iW - w - 1 || x < 0)
    changeX *= -1;

  y += changeY;
  if (y == iH - h - 1 || y < 1)
    changeY *= -1;

  sprite.pushImage(0, 0, 320, 170, imageS);


  sprTemperature.fillRoundRect(0, 0, 86, 20, 3, TFT_WHITE);

  sprTemperature.pushImage(0, 120, 86, 40, logo);

  sprTemperature.drawRoundRect(0, 26, 86, 44, 3, TFT_WHITE);
  sprTemperature.drawRoundRect(0, 92, 86, 24, 3, TFT_WHITE);

  sprTemperature.setTextColor(TFT_BLACK, TFT_WHITE);
  sprTemperature.drawString("TEMP", 40, 10, 2);
  sprTemperature.drawCircle(76, 36, 3, TFT_WHITE);
  sprTemperature.setTextColor(TFT_WHITE, TFT_BLACK);

  temperature = String(bmp.readTemperature());
  sprTemperature.drawString(temperature.substring(0, 4), 36, 40);
  sprTemperature.setTextFont(0);
  sprTemperature.drawString("CELSIUS", 26, 62);

  pressure = bmp.readPressure();
  sprTemperature.drawString("PRESSURE  ", 40, 80, 2);

  sprTemperature.drawString(String(pressure / 1000) + " hPascal", 44, 104, 2);
  sprTemperature.pushToSprite(&sprite, xt, yt, TFT_BLACK);
  sprite.pushSprite(0, 0);
}