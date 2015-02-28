
#include <Adafruit_NeoPixel.h>

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#include <FHT.h>

#define PIN 10
const int COLUMNS = 7;
const int ROWS = 7;

unsigned long count = 0;

double maximums[COLUMNS] ;
double outputArray[COLUMNS];

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
  
//  Serial.println("start");
//    for (byte i = 0 ; i < FHT_N/2 ; i++) {
//      Serial.println(fht_log_out[i]); // send out the data
//    }
    
  
  int val = (FHT_N / 2 - 2) / COLUMNS;
  double total = 0;
  for (int i = 2; i < FHT_N / 2; i++) {
//      Serial.println(total);

    if ((i - 2) % val != 0) {
//      Serial.print(i);
//      Serial.print(" ");
//      Serial.println(fht_log_out[i]);
      total =+ fht_log_out[i];
    } else {
      int index = (i - 2) / val;
//      Serial.print(index);
//      Serial.print(" ");

      double newAverage = total / val;
      
//      Serial.println(total);
      
      
      if (newAverage > maximums[index]) {
        maximums[index] = newAverage;
      }
//      Serial.println(newAverage);
      if (index == 0) {
//        Serial.println(total);
      }
      outputArray[index] = newAverage / maximums[index];
//              Serial.println(maximums[index]);
      total = 0;
    }
  }

//  for (int i = 0; i < COLUMNS; i++) {
//    outputArray[i] = 1;
//  }
//  Serial.println(millis());
  
//  Serial.write(255); // send a start byte
//  Serial.write(outputArray, (FHT_N/2));
  
//    Serial.write(fht_log_out, (FHT_N/2)); // send out the data
    
  if (millis() - count > 50){
    setColumn();
    strip.show();
    count = millis();
  }

  
//  delay(50);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
//        strip.setPixelColor(0, 50);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void setColumn() {

  for (int i = 0; i < COLUMNS; i++) {
    int targetHeight = round(outputArray[i] * ROWS);
//    Serial.print("column ");
//    Serial.print(i);
//    Serial.print(" = ");
//    Serial.println(targetHeight);
    for (int j = 0; j < ROWS; j++) {
      int currentPixel = i * ROWS + j;
      if (i % 2 == 0) {
        //count up
        if (currentPixel < (i * ROWS) + targetHeight) {
          int r = 255;
          int g = 0;
          int b = 0;
            
            
          strip.setPixelColor(currentPixel, strip.Color(g, r, b));
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      } else {
        //count down
        if (currentPixel % ROWS >= ROWS - targetHeight) {
          int r = 255;
          int g = 0;
          int b = 0;
          
          strip.setPixelColor(currentPixel, strip.Color(g, r, b));
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      }
    }
  }
}

