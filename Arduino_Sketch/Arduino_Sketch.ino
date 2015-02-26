#include <Adafruit_NeoPixel.h>
#include <FHT.h>

#define LOG_OUT 1 // use the log output function
#define FHT_N 16 // set to 256 point fht

#define PIN 10
int COLUMNS = 5;
int ROWS = 7;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ROWS * COLUMNS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  int val = (FHT_N - 2) / 2;
  double array[];
  for (int = 2; i < FHT_N; i++) {
    int total = 0;
    
  }
  
  setColumn(array);
  strip.show();
  delay(1000);

}

void runFHT() {
  for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
  }
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_log(); // take the output of the fht
  sei();
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
  for (int i = 0; i < COLUMNS; i++) {
    int targetHeight = round(array[i] * ROWS);
    for (int j = 0; j < ROWS; j++) {
      int currentPixel = i * ROWS + j;
//      Serial.println(currentPixel);
      if (i % 2 == 0) {
        //count up
        if (currentPixel < i * ROWS + targetHeight) {
          strip.setPixelColor(currentPixel, 255);
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      } else {
        //count down
        if (currentPixel % ROWS >= ROWS - targetHeight) {
          strip.setPixelColor(currentPixel, 255);
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      }
    }
  }
}

