#include <Adafruit_NeoPixel.h>

int COLUMNS_ENABLED = 2;
int COLUMN_HEIGHT = 7;

int DATA_PIN = 10;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(COLUMNS_ENABLED * COLUMN_HEIGHT, DATA_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
