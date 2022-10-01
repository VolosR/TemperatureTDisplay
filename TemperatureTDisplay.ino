#include "TFT_eSPI.h"
#include "image.h"
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
TFT_eSPI tft= TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite spr2 = TFT_eSprite(&tft); //sprite for temperature


int iW=480;
int iH=244;

int w=320;
int h=170;
int m=w;

int xt=8;
int yt=8;

int start=1;
unsigned short imageS[54400]={0};
int pos=0;
int x=0;
int y=30;
int changeX=1;
int changeY=1;

String temperature;
long pressure;

void setup() {
tft.init();
tft.setRotation(1);
tft.setSwapBytes(true);
sprite.createSprite(320,170);
sprite.setSwapBytes(true);

    spr2.createSprite(86,160);
    spr2.fillSprite(TFT_GREEN);
    spr2.setTextDatum(4);
    spr2.setTextColor(TFT_WHITE,TFT_GREEN);
    spr2.setFreeFont(&Orbitron_Light_24);

Wire.begin(43,44);
bmp.begin();
}

void loop() {

  
  spr2.fillSprite(TFT_BLACK);
  spr2.setFreeFont(&Orbitron_Light_24);
  pos=x+iW*y;
  start=pos;
  m=w+pos;
  for(int i=0;i<54400;i++)
  {
  if(start%m==0)
  {
  start += iW-w;
  m += iW;
  }
  imageS[i]=picture[start];
  start++;
  
  }
  x += changeX;
  if(x==iW-w-1 || x<0)
  changeX *= -1;

   y += changeY;
  if(y==iH-h-1 || y<1)
  changeY *= -1;
  
  sprite.pushImage(0,0,320,170,imageS);

  
  spr2.fillRoundRect(0,0,86,20,3,TFT_WHITE);

  spr2.pushImage(0,120,86,40,logo);
  
  spr2.drawRoundRect(0,26,86,44,3,TFT_WHITE);
  spr2.drawRoundRect(0,92,86,24,3,TFT_WHITE);

  spr2.setTextColor(TFT_BLACK,TFT_WHITE);
  spr2.drawString("TEMP",40,10,2);
  spr2.drawCircle(76,36,3,TFT_WHITE);
  spr2.setTextColor(TFT_WHITE,TFT_BLACK);

  temperature=String(bmp.readTemperature());
  spr2.drawString(temperature.substring(0,4),36,40);
  spr2.setTextFont(0);
  spr2.drawString("CELSIUS",26,62);

  long pressure=bmp.readPressure();
  spr2.drawString("PRESSURE  ",40,80,2);
  
  spr2.drawString(String(pressure/1000)+" hPascal",44,104,2);
  spr2.pushToSprite(&sprite,xt,yt,TFT_BLACK);
  sprite.pushSprite(0,0);

}
