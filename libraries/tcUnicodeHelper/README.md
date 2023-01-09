# TcUnicode support for Adafruit_GFX, U8G2, TFT_eSPI, tcMenu.

## Summary

TcUnicode provides Unicode character support to a wide range of display libraries (Adafruit_GFX, U8G2, TFT_eSPI, and also TcMenu). There is no need to use special versions of the libraries to use TcUnicode, and there is even a UI for creating the fonts. 

Dave Cherry / TheCodersCorner.com made this framework available for you to use. It takes me significant effort to keep all my libraries current and working on a wide range of boards. Please consider making at least a one off donation via the sponsor button if you find it useful.

In any fork, please ensure all text up to here is left unaltered.

## Why this library and this format?

This format and library were originally devised as part of the tcMenu project specifically for mbed boards, I thought the format to be very useful, enough to make open to all. It provides a very extensible unicode format that works across many libraries and boards.

This library provides the full implementation of TcUnicode for use in many different environments that are listed below.

* For TcMenu Device Drawable - the designer supports this format natively.
* Most Adafruit_GFX libraries - see the packaged Adafruit example.
* U8G2 library - although the library has good Unicode support, this is another option. See the packaged example.
* TFT_eSPI - although the library has good Unicode support, this is another option. See the packaged example.

## Includes a UTF-8 decoder for mbed and Arduino

This package includes a strict asynchronous UTF8 decoder that needs very little memory to operate with, it is backed by a series of tests that ensure it is quite strict. Given the way it operates it is possible to use it with the Print interface as it can push one char at a time. Safely bailing out if a stream is interrupted midway.

## Creating tcUnicode or custom Adafruit GFX fonts

There is a [custom UI for creating tcUnicode and Adafruit fonts](https://www.thecoderscorner.com/products/arduino-libraries/tc-menu/using-custom-fonts-in-menu/#creating-a-unicode-or-adafruit-font-using-the-designer-ui) built into the [tcMenu Designer application](https://github.com/davetcc/tcMenu/releases) in V3.0 and above. It works by loading a font, extracting the bitmaps at a given size, and allowing user selection of the available fonts.

TcUnicode is better for representing fonts with a wide range of glyphs from different Unicode groups, as each glyph stores the code, it is far more efficient at dealing with situations where only a few glyphs from each block are present.

## TextPipelines

The way we've implemented the interface between primitive drawing and the Unicode handler means in future we can provide transformations, for example a rotation transformation. For now, they only provide the direct support for drawing on each display type.

## How does this support work?

Firstly, you can create fonts by generating from a desktop font file directly from "tcMenu Designer" UI on most desktop platforms, and then they are included into your project as a header file.

There is a custom dialog within "TcMenu Designer UI" where you can select the Unicode blocks, and even sub ranges within blocks that you want to support using the UI.

Please raise all questions in the main TcMenu repository discussions.

## Questions and documentation

You can ask questions either in the [discussions section of the tcMenu repo](https://github.com/davetcc/tcMenu/discussions), or using the Arduino forum. We generally answer most questions, but the rules of engagement are: **this is my hobby, I make it available because it helps others**. Don't expect immediate answers, make sure you've recreated the problem in a simple sketch that you can send to me. Please consider making at least a one time donation using the sponsor link if we do help you out.

* [UTF-8 decoder and Unicode font documentation on TheCodersCorner.com](https://www.thecoderscorner.com/products/arduino-libraries/tc-unicode-helper/)
* [discussions section of the tcMenu repo](https://github.com/davetcc/tcMenu/discussions) of tcmenu git repo
* [Arduino discussion forum](https://forum.arduino.cc/) where questions can be asked, please tag me using `@davetcc`.
* [Legacy discussion forum probably to be made read only soon](https://www.thecoderscorner.com/jforum/).
