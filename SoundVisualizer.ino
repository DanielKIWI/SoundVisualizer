#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

#define LED_PIN 3
#define SOUND_PIN A3
#define NUM_LEDS 27
#define BRIGHTNESS 20



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

uint8_t red = 0;
uint8_t green = 50;
uint8_t blue = 0;
uint8_t white = 0;

uint32_t color = strip.Color(red, green, blue, white);
uint32_t scaledColor(float factor) {
  return strip.Color(red * factor, green * factor, blue, white * factor);
}

uint32_t backgroundColor = strip.Color(0, 0, 0, 0);

struct LedLine {
  int* IndexMap;
  int count;
  int minValue;
  int maxValue;
  int deltaValue;
  LedLine(int* iMap, int c, int minV, int maxV) {
    IndexMap = iMap;
    count = c;
    maxValue = maxV;
    minValue = minV;
    deltaValue = (maxValue - minValue);
  }
  void vizualizeVolume(int volume){
    float x = (float)((volume - minValue) * count) / (float)deltaValue;
    int i = 0;
    while (x > 1 && i < count) {
      strip.setPixelColor(IndexMap[i], color );
      i++;
      x--;
    }
    if (i < count && x > 0) {
      strip.setPixelColor(IndexMap[i], scaledColor(x) );
      i++;
    }
    while (i < count) {
      strip.setPixelColor(IndexMap[i], backgroundColor );
      i++;
    }
  }
};

LedLine middleLineLeft = LedLine(new int[4]{ 13, 12, 11, 10}, 4, 0, 255);
LedLine middleLineRight = LedLine(new int[4]{ 13, 14, 15, 16} , 4, 0, 255);
LedLine leftLineOut = LedLine(new int[5]{ 8, 7, 4, 3, 0 }, 5, 400, 800);
LedLine leftLineIn = LedLine(new int[5]{ 9, 6, 5, 2, 1 }, 5, 255, 1023);
LedLine rightLineIn = LedLine(new int[5]{ 17, 20, 21, 24, 25 }, 5, 255, 800);
LedLine rightLineOut = LedLine(new int[5]{ 18, 19, 22, 23, 26 }, 5, 400, 1023);

int sensorValue = 0;
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
float simVal = 128;
float dV = 0;
void loop() {
  // put your main code here, to run repeatedly:
/*
  dV += random(-simVal, 255 - simVal) / 255.0f;
  if (dV > 10) dV = 10;
  else if (dV < -10) dV = -10;
  simVal += dV;
  if (simVal > 1023) simVal = 1023;
  else if (simVal < 0) simVal = 0;
  
  if (Serial.available()) {
    simVal = Serial.read();
  }
  */
  sensorValue = analogRead(SOUND_PIN);
  //sensorValue = simVal;//analogRead(SOUND_PIN);
  Serial.println(sensorValue,  DEC);
  
  middleLineLeft.vizualizeVolume(sensorValue);
  middleLineRight.vizualizeVolume(sensorValue);
  leftLineOut.vizualizeVolume(sensorValue);
  leftLineIn.vizualizeVolume(sensorValue);
  rightLineIn.vizualizeVolume(sensorValue);
  rightLineOut.vizualizeVolume(sensorValue);
  strip.show();
  delay(5);
}
