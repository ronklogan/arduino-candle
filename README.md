# Arduino-candle

Simple little candle-flame code for an Arduino-compatible board and a Neopixel ring or strip.

## YouTube Demonstration

(add link here)

## Components

* Adafruit Pro Trinket 5V board: https://www.adafruit.com/product/2000
* Adafruit Pro Trinket LiPoly Backback: https://www.adafruit.com/product/2124
* Adafruit 12-pixel NeoPixel ring: https://www.adafruit.com/product/1643
* LiPo battery 500mAh: https://www.adafruit.com/product/1578

Any LiPo battery capacity would do; obviously the more capacity, the longer the candle will "burn" on battery power. I just happened to have a 500mAh LiPo lying around.

And really, just about any Arduino-compatible board will do. I just happened to have a Pro Trinket handy. I'm sure an [Adafruit GEMMA](https://www.adafruit.com/product/3501) or a [Sparkfun Pro Micro](https://www.sparkfun.com/products/12640) or just about anything else will work. The NeoPixel needs only a single digital-out pin to control all its pixels, so that's a pretty low bar for the board requirements. The NeoPixels will typically work with a 3.3v logic board, but a 5V board would be best.

## Schematic

(Could use a picture or diagram here, TBH)

Attach the LiPoly backpack to the Pro Trinket. With the USB connector at the top, the top-right three pins on the Pro Trinket attach to the three pins on the backpack such that the backpack is on top of the Pro Trinket with the LiPo connector facing down.

* LiPoly BAT -> Pro Trinket BAT
* LiPoly G -> Pro Trinket G
* LiPoly 5V -> Pro Trinket BUS
* NeoPixel POWER -> Pro Trinket 5V
* NeoPixel GND -> Pro Trinket G
* NeoPixel INPUT -> Pro Trinket 6

Attach the LiPo battery to the LiPoly backpack - the NeoPixel will drive from the battery. With the battery connected, attach a USB to the Pro Trinket connector and the LiPo battery will charge. Disconnect the USB to resume running off the battery.

If you want to add a power switch to your project, cut the jumper between the two holes on the LiPoly backpack that have a white square printed around them. Attach a switch between the two pin holes - when the switch is off, no power will be provided by the battery to the Pro Trinket board.

If budget is a concern and/or you don't care if it needs to be plugged in all the time, the LiPo battery and LiPoly backpack aren't necessary - they just give a little portability to the project.

## Code

The code assumes the NeoPixel Input is attached to "Pin 6" on the board. If you are using a different board, or otherwise attach the NeoPixel to another pin, change the `PIN_OUT` define at the top of the code.

The code assumes we are working with a 12-pixel NeoPixel strip or ring. If you are using a strip or ring with a different number of pixels, change the `NUMBER_OF_PIXELS` define at the top of the code.

Feel free to play with the define values of the `BETA` (percentage value between 0 and 100) and `FLICKER_DELAY` (in milliseconds) in order to get an effect that looks best to you.

The code works with HSV color values: Hue, Saturation, and Value. The _hue_ is the color of the flame. The code uses a random value between 45 (orange) and 55 (yellow). Feel free to experiment with these values to change the color of your candle flame. This hue value is an angle in degrees between 0 and 360. Red is around 0 degrees, Green is around 120 degreen, and blue is around 240.

## Programming the Pro Trinket

The Pro Trinket board doesn't really have a true USB connector. To program it:

* Download/paste the code into your Arduino IDE.
* Connect the Pro Trinket board via USB to your computer.
* In the IDE, the "Board" setting should be "Pro Trinket 5V/16MHz (USB)."
* Set the "Programmer" to "USBtinyISP."
* Press the reset button on the Pro Trinket - when the red onboard LED starts to pulse, begin uploading the code from the IDE to the board.

See the [Adafruit Pro Trinket tutorial](https://learn.adafruit.com/introducing-pro-trinket) for more information on how to set that board up and program it. If your Arduino IDE doesn't have those board/programmer options available, you'll probably need to install some drivers and/or other packages from the Library Manager. Don't forget to add the NeoPixel library!

Have fun!
