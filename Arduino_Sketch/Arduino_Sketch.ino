
#include <Adafruit_NeoPixel.h>

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#include <FHT.h>

#define PIN 10
const int COLUMNS = 7;
const int ROWS = 7;

unsigned long timer = 0;

int oldR = 255;
int oldG = 0;
int oldB = 0;

double maximum;
double outputArray[COLUMNS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ROWS * COLUMNS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  
  Serial.begin(115200);
  
  //FHT initialization
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  
  //NeoPixel initialization
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'


//  colorWipe(strip.Color(255, 0, 0), 30); // Red
//  colorWipe(strip.Color(0, 255, 0), 30); // Green
//  colorWipe(strip.Color(0, 0, 255), 30); // Blue
}

void loop() {

  //FHT Loop
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
    
  //Averaging the FHT results
  int binsToAverage = (FHT_N / 2) / COLUMNS;
  double total = 100;
  for (int i = 2; i <= FHT_N / 2; i++) {
//      Serial.println(total);

    if (i % binsToAverage != 0) {
//      Serial.print(i);
//      Serial.print(" ");
//      Serial.println(fht_log_out[i]);
      total =+ fht_log_out[i];
    } else {
      int index = i / binsToAverage;
//      Serial.print(index);
//      Serial.print(" ");

      double newAverage = total / binsToAverage;
      
//      Serial.println(total);
      
      
      if (newAverage > maximum) {
        maximum = newAverage;
      }
//      Serial.println(newAverage);
      if (index == 0) {
        Serial.println(total);
      }
//      Serial.print("outputArray[");
//      Serial.print(index);
//      Serial.print("] = ");
//      Serial.println(newAverage/maximum);
      outputArray[index - 1] = newAverage / maximum;
//              Serial.println(maximums[index]);
      total = 0;
    }
  }

//  for (int i = 0; i < COLUMNS; i++) {
//    outputArray[i] = 0.5;
//  }
//  Serial.println(millis());
  
//  Serial.write(255); // send a start byte
//  Serial.write(outputArray, (FHT_N/2));
  
//    Serial.write(fht_log_out, (FHT_N/2)); // send out the data
    
    if (millis() - timer > 50) {
      setColumn();
      strip.show();
      timer = millis();

//      Serial.println("start");
//      for (int i = 0; i < COLUMNS; ++i) {
//      	Serial.println(outputArray[i]);
//      }
//      if (millis() - colourChangeDelayTimer > 100) {
//        oldR = (oldR + 85) % 256;
//        oldG = (oldG + 170) % 256;
//        oldB = (oldB + 255) % 256;
//      }
     
      
    }


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
//        Serial.print(currentPixel);
//        Serial.print(" ");
//        Serial.println((i * ROWS) + targetHeight);
        
        if (currentPixel < (i * ROWS) + targetHeight) {

          int r = oldR;
          int g = oldG;
          int b = oldB;
            
            
          strip.setPixelColor(currentPixel, strip.Color(g, r, b));
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      } else {
        //count down
        if (currentPixel % ROWS >= ROWS - targetHeight) {
          int r = oldR;
          int g = oldG;
          int b = oldB;
          
          strip.setPixelColor(currentPixel, strip.Color(g, r, b));
        } else {
          strip.setPixelColor(currentPixel, 0);
        }
      }
    }
  }
}

