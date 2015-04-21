OPC opc;
final int LEDS_PER_COLUMN = 20;

void setup()
{
  opc = new OPC(this, "127.0.0.1", 7890);
  frameRate(10);
//  colorMode(RGB, 100);
}

void draw()
{
  // RAINBOW FADE!!!!!
  for (int i = 0; i < 240; i++) {
//    float hue = (millis() * 0.01 + i * 2.0) % 100; 
    opc.setPixel(i, color(0, 255, 0));
      opc.writePixels();
       delay(100);


  }
//  for (int i = 0; i < 60; i++) {
////    float hue = (millis() * 0.01 + i * 2.0) % 100; 
//    opc.setPixel(i, color(0, 0, 0));
//  }
//  opc.writePixels();
//  color[][] array = new color[47][20];
  
  

  // When you haven't assigned any LEDs to pixels, you have to explicitly
  // write them to the server. Otherwise, this happens automatically after draw().
}

//Lights up the board and refreshes according to the 2d array of colors
void display(color[][] input) {
  for (int column = 0; column < input.length; column++) {
    //if the column is even: regular direction
    if (column % 2 == 0) {
      for (int row = 0; row < input[0].length; row++) {
        int pixelIndex = column * LEDS_PER_COLUMN + row;
        opc.setPixel(pixelIndex, input[column][row]);
      }
    } // if the column is odd: reverse it
      else {
      for (int row = input[0].length - 1; row >= 0; row--) {
        int pixelIndex = column * LEDS_PER_COLUMN + row;
        opc.setPixel(pixelIndex, input[column][row]);
      }
    }
  }
  opc.writePixels();
}

void display(int[] input) {
  for (int index = 0; column < input.length; index++) {
      opc.setPixel(index, input[column][row]);
    }
  }
}
