# LedSync Arduino Library

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

## Introduction

This library allows you to blink as many LEDs and NeoPixels as you want synchronously without interfering each other progress.



## Dependencies

This library has two dependencies:

* [Adafruit_NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel) created by [Adafruit Industries](https://github.com/adafruit)
* [LinkedList](https://github.com/ivanseidel/LinkedList) created by [ivanseidel](https://github.com/ivanseidel)

You must have them installed in your environment in order for it to compile.



## Getting started



### Creating a LED Object

[LED Class](https://github.com/evert-arias/LedSync/blob/master/docs/Led.md)

This object represents a LED with all its methods and properties. You must create one of this objects for each connected LED and pass the pin number where the LED is connected as an argument of the constructor method of the class. 

``` c++
Led myLed(pin, polarity);
```



### Creating NeoPixel Object

 [NeoPixel Class](https://github.com/evert-arias/LedSync/blob/master/docs/NeoPixel.md)

This object represents a NeoPixel with all its methods and properties. You must create one of this objects for each NeoPixel. The order in which these objects are created must match their position in the NeoPixelStrip. 

``` c++
NeoPixel myPixel;
```



### Adding Objects to LedSync

LedSync is the core processor object, you must add to it, all instances of LEDs and NeoPixels. Don't forget to pass them as reference, otherwise it won't compile.

``` c++
void setup(){
  // Add the LED Object
  LedSync.add(&myLed);
  // Add the NeoPixel Object
  LedSync.add(&myPixel);
}
```



### Feed it 

In order for LedSync to work, it must have a feeding from the program loop, so don't forget to always call ```LedSync.update()``` in the loop.

```` c++
void loop(){
  LedSync.update();
}
````



## That's it!  

You are now ready to start calling the methods of your LEDs and NeoPixels.

[LED Class](https://github.com/evert-arias/LedSync/blob/master/docs/Led.md)

[NeoPixel Class](https://github.com/evert-arias/LedSync/blob/master/docs/NeoPixel.md)

[Color Class](https://github.com/evert-arias/LedSync/blob/master/docs/Color.md)

[LedSync Class](https://github.com/evert-arias/LedSync/blob/master/docs/LedSync.md)





## Copyright

[MIT](../LICENSE.md) © [Evert Arias](https://evert.ariascode.com/about)


