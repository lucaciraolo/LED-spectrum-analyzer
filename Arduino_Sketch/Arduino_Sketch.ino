#include <Adafruit_NeoPixel.h>

#define PIN 10
int COLUMNS = 2;
int ROWS = 7;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ROWS * COLUMNS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  double array[] = {0.5, 0.1};
  setColumn(array);
  strip.show();
  delay(1000);

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void setColumn(double array[]) {
  for (int i = 0; i < sizeof(array); i++) {
    int targetHeight = round(array[i] * ROWS);
    for (int j = 0; j < ROWS; j++) {
      int currentPixel = i * ROWS + j;
      if (i % 2 == 0) {
        //count up
        if (currentPixel < i * ROWS + targetHeight) {
          strip.setPixelColor(currentPixel, 255);
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      } else {
        //count down
        if (currentPixel % ROWS == ROWS - targetHeight) {
          strip.setPixelColor(currentPixel, 255);
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      }
    }
  }
}

