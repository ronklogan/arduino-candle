/*
MIT License

Copyright (c) 2020 Ron Logan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Adafruit_NeoPixel.h>

// output pin to which the neopixel strip input is attached
#define PIN_OUT 6

// the number of pixels in the neopixel ring/strip
#define NUMBER_OF_PIXELS 12

// beta value of low-pass algorithm, in percent
#define BETA 30L

// loop delay, in milliseconds
#define FLICKER_DELAY 50

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(
  NUMBER_OF_PIXELS,
  PIN_OUT,
  NEO_GRB + NEO_KHZ800);

uint8_t lowPass(uint8_t current, uint8_t target) {
  return (BETA * target + (100 - BETA) * current) / 100;
}

uint32_t randomColor() {
  // generate a new random target color value using the HSV space (HUE, SATURATION, VALUE)
  // in HSV, orange is around 45 degrees, and yellow about 60 degrees.
  // Neopixel ColorHSV method takes H as a value from 0 (0deg) to 65535 (360deg),
  // so generate a random hue value somewhere between the orange and yellow percentage
  // and convert to the range 0-65535
  uint16_t H = random(45, 55) * 65535L / 360;

  // saturation (color vs gray-scale equivalent) is 0% (no color) to 100% (vivid color)
  // Neopixels take a value from 0 (no color) to 255 (color), so generate a random number
  // in the desirted range and convert to 0-255
  uint8_t S = random(50, 100) * 255L / 100;

  // value is the brightness. 0% (off) to 100% (bright on). Neopixels take a byte
  // between 0 (off) and 255 (on), so generate a random number over the entire range
  uint8_t V = random(0, 255);

  // create the 32-bit color integer from the HSV components
  return strip.gamma32(strip.ColorHSV(H, S, V));
}

// go through each pixel and set it to a random candle-color
void reset() {
  for(int pixelIndex = 0; pixelIndex < NUMBER_OF_PIXELS; ++pixelIndex) {
    strip.setPixelColor(pixelIndex, randomColor());
  }
}

// just a structure to get at the individual bytes of a 32-bit value
typedef struct {
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
  uint8_t byte4;
} ColorBytes;

void flicker() {
  // for each neopixel....
  for(int pixelIndex = 0; pixelIndex < NUMBER_OF_PIXELS; ++pixelIndex) {
    // generate a new random target color value against which we
    // will adjust the current LED color value using a low-pass filter algo.
    // We want to low-pass filter each individual byte of the RGB value
    uint32_t target = randomColor();
    ColorBytes* targetBytes = (ColorBytes*)&target;

    // get the current value of the pixel and a byte-pointer to the value
    uint32_t color = strip.getPixelColor(pixelIndex);
    ColorBytes* colorBytes = (ColorBytes*)&color;

    // adjust each byte from the current value to the target value using
    // a low-pass algorithm
    colorBytes->byte1 = lowPass(colorBytes->byte1, targetBytes->byte1);
    colorBytes->byte2 = lowPass(colorBytes->byte2, targetBytes->byte2);
    colorBytes->byte3 = lowPass(colorBytes->byte3, targetBytes->byte3);
    colorBytes->byte4 = lowPass(colorBytes->byte4, targetBytes->byte4);

    // set the color of the neopixel to this new value
    strip.setPixelColor(pixelIndex, color);
  }
}

void setup() {
  // start the strip, reset to some random HSV values
  // for the initial state of the lights, and show it
  strip.begin();
  reset();
  strip.show();
}

void loop() {
  // adjust each neopixel, show them, and wait a little before the next iteration
  flicker();
  strip.show();
  delay(FLICKER_DELAY);
}
