OPC opc;
int width = 7;
int height = 7;

void setup()
{
  opc = new OPC(this, "127.0.0.1", 7890);
  opc.setDithering(false);
  opc.setInterpolation(false);

//  opc.led(0, 0, 0);
}

void draw()
{
  for (int i = 0; i < 49; i++) {
    lightup(i);
    opc.writePixels();
    delay(50);
  }
}

void lightup(int index) {
  clear();
  opc.setPixel(index, color(0, 255, 0));
}

void clear() {
  for (int i = 0; i < 49; i++) {
    opc.setPixel(i, color(0, 0, 0));
  }
}
