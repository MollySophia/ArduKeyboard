#include <Keyboard.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LATCH 20
#define CLOCK 19
#define DATA 21
int pinRow[8] = {2, 3, 4, 5, 6, 7, 8, 9};
#define OLED_DC  1
#define OLED_RST  0

Adafruit_SSD1306 display(OLED_DC, OLED_RST, 14);

bool matrix[16][8];
char keymap[16][8] = {
  {0, 0, 0, 0, 0, 0, 0, 215},
  {0, 0, 0, 0, 0, 218, 0, 216},
  {0, 57, 117, 0, 108, 0, 44, 0},
  {54, 55, 111, 121, 106, 104, 110, 98},
  {0, 51, 101, 0, 100, 0, 99, 0},
  {96, 49, 113, 179, 97, 0, 122, 0},
  {0, 0, 0, 129, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 131, 0, 0},
  {0, 0, 0, 0, 0, 32, 0, 217},
  {0, 0, 0, 130, 0, 0, 0, 0},
  {128, 0, 0, 0, 0, 0, 0, 0},
  {0, 50, 119, 193, 115, 0, 120, 0},
  {53, 52, 114, 116, 102, 103, 0, 118},
  {61, 56, 105, 92, 107, 0, 109, 0},
  {45, 48, 112, 47, 176, 0, 59, 46},
  {0, 0, 0, 178, 0, 0, 133, 0}
};


void handleMatrix(int x, int y) {
  if (matrix[x][y] == 0) {
    Keyboard.press(keymap[x][y]);
  }
  else {
    Keyboard.release(keymap[x][y]);
  }
}

void scanMatrix() {
  for (int i = 0; i < 16; ++i) {
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLOCK, MSBFIRST, (~(1 << i) >> 8));
    shiftOut(DATA, CLOCK, MSBFIRST, ~(1 << i));
    digitalWrite(LATCH, HIGH);
    for (int j = 0; j < 8; ++j) {
      if (digitalRead(pinRow[j]) != matrix[i][j]) {
        delay(5);
        if (digitalRead(pinRow[j]) != matrix[i][j]) {
          matrix[i][j] = digitalRead(pinRow[j]);
          handleMatrix(i, j);
        }
      }
    }
  }
}

void setup() {
  Keyboard.begin();
//  Serial.begin(115200);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < 8; ++i) {
    pinMode(pinRow[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 8; ++j)
      matrix[i][j] = 1;
  
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void loop() {
  // keyboard
  scanMatrix();
}
