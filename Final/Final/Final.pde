import ddf.minim.analysis.*;
import ddf.minim.*;

Minim minim;  
AudioPlayer audioInput;
FFT fftLin;
OPC opc;

final int LEDS_PER_COLUMN = 20;
final int LEDS_PER_ROW = 47;

final int LED_PIXEL_WIDTH = 10;
final int LED_PIXEL_HEIGHT = 10;
final int LED_PIXEL_HORIZONTAL_SPACING = 20;
final int LED_PIXEL_VERTICAL_SPACING = 20;

final int BOARD_REFRESH_DELAY = 0;
long lastBoardRefresh = 0;

int countColumn = 0;
int countRow = 0;

void setup()
{
  background(0);
  colorMode(HSB, 100);
   size((LED_PIXEL_WIDTH + LED_PIXEL_HORIZONTAL_SPACING) * LEDS_PER_ROW,
         (LED_PIXEL_HEIGHT + LED_PIXEL_VERTICAL_SPACING) * LEDS_PER_COLUMN);

  opc = new OPC(this, "127.0.0.1", 7890);
 
  minim = new Minim(this);
  audioInput = minim.loadFile("Seve.mp3");
  audioInput.loop();
  
  fftLin = new FFT( audioInput.bufferSize(), audioInput.sampleRate() );
//  fftLin.linAverages( LEDS_PER_ROW );
  fftLin.logAverages( 10, 4 );
  
  rectMode(CORNER);

}

void draw()
{
  //Perform the FFT
  fftLin.forward( audioInput.mix );
  
  color[][] pixelArray = new color[LEDS_PER_ROW][LEDS_PER_COLUMN];
  
  //Populate the pixelArray with the FFT results
  for (int column = 0; column < pixelArray.length; column++) {
//    println(fftLin.getAvg(column));
    int lightBarHeight;
    if (column < (LEDS_PER_ROW / 2)) {
      lightBarHeight = Math.round(fftLin.getAvg(column) * 0.25);
    } else {
      lightBarHeight = Math.round(fftLin.getAvg(column) * 0.5);
    }

    for (int row = 0; row < pixelArray[0].length; row++) {
      if (row < lightBarHeight) {
        float hue = (millis() * 0.01 * 2.0) % 100; 
        pixelArray[column][row] = color(hue, 40, 80);
      } else {
        pixelArray[column][row] = color(0, 0, 0);
      }
    }
  }
//  for (int column = 0; column < pixelArray.length; column++) {
//    int lightBarHeight = 5;
//    for (int row = 0; row < pixelArray[0].length; row++) {
//      if (row < lightBarHeight) {
//        pixelArray[column][row] = color(255, 0, 0);
//      } else {
////        opc.setPixel(column * LEDS_PER_COLUMN + row, color(0, 0, 0));
//        pixelArray[column][row] = color(0, 0, 0);
//      }
//    }
//  }
  
  
  if (millis() - lastBoardRefresh > BOARD_REFRESH_DELAY) {
//    if (countRow == LEDS_PER_ROW) {
//      countColumn += 1;
//    }
//    countRow += 1;
//    opc.setPixel(513, color(255, 0, 0));
//    opc.writePixels();
    display(pixelArray);
    displayOnScreen(pixelArray);
    lastBoardRefresh = millis();
  }
}
//displays the color array on screen
void displayOnScreen(color[][] input) {
  noStroke();
  for (int column = 0; column < input.length; column++) {
    int x = column * (LED_PIXEL_WIDTH + LED_PIXEL_HORIZONTAL_SPACING);
    for (int row = input[0].length - 1; row >= 0; row--) {
        fill(input[column][row]);
        if (input[column][row] == color(0, 0, 0)) {
          fill(color(0, 0, 0));
        }
        int y = (input[0].length - row) * (LED_PIXEL_HEIGHT + LED_PIXEL_VERTICAL_SPACING);
        rect(x, y, LED_PIXEL_WIDTH, LED_PIXEL_HEIGHT);
    }
  }
}

//Lights up the board and refreshes according to the 2d array of colors
void display(color[][] input) {
  for (int column = 0; column < input.length; column++) {
    //if the column is even: regular direction
    if (column % 2 == 0) {
      for (int row = 0; row < input[0].length; row++) {
        int pixelIndex = column * 32 + row;
        opc.setPixel(pixelIndex, input[column][row]);
      }
    } // if the column is odd: reverse it
      else {
      for (int row = input[0].length - 1; row >= 0; row--) {
        int pixelIndex = ((int) Math.floor(column / 2)) * 64 + (LEDS_PER_COLUMN + (LEDS_PER_COLUMN - row - 1));
        opc.setPixel(pixelIndex, input[column][row]);
      }
    }
  }
  opc.writePixels();
}

void display(color[] input) {
  for (int index = 0; index < input.length; index++) {
      opc.setPixel(index, input[index]);
   }
}
