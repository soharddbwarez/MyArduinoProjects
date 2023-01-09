
#ifndef TC_UNICODE_FONTHELPER_H
#define TC_UNICODE_FONTHELPER_H

#include <string.h>
#include <inttypes.h>
#include "Utf8TextProcessor.h"
#include "UnicodeFontDefs.h"

#if !defined(pgm_read_dword) && defined(__MBED__)
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))
#define pgm_read_ptr(addr) (*(addr))
#define memcpy_P memcpy
#endif // pgm_read_byte

/**
 * @file tcUnicodeHelper.h
 * @brief contains a Unicode handler that can process UTF-8 data and print it onto most display types.
 */

#if __has_include ("zio_local_definitions.h")
#    include "zio_local_definitions.h"
#endif

#ifndef TC_COORD_DEFINED
#define TC_COORD_DEFINED

namespace tcgfx {

/** A structure that holds both X and Y direction in a single 32 bit integer. Both x and y are public */
    struct Coord {
        /** default construction sets values to 0 */
        Coord() {
            x = y = 0;
        }

        /**
         * Create a coord based on an X and Y location
         * @param x the x location
         * @param y the y location
         */
        Coord(int x, int y) {
            this->x = x;
            this->y = y;
        }

        Coord(const Coord &other) = default;
        Coord& operator = (const Coord& other) = default;

        int16_t x;
        int16_t y;
    };
}

#endif // TC_COORD_DEFINED

using namespace tcgfx;

class TextPlotPipeline {
public:
    virtual ~TextPlotPipeline() = default;
    virtual void drawPixel(uint16_t x, uint16_t y, uint32_t color) = 0;
    virtual void setCursor(const Coord& where) = 0;
    virtual Coord getCursor() = 0;
    virtual Coord getDimensions() = 0;
};

#if __has_include (<graphics/GraphicsDeviceRenderer.h>)
#include <graphics/GraphicsDeviceRenderer.h>
#define UNICODE_TCMENU_GRAPHIC_DEVICE_AVAILABLE
class DrawableTextPlotPipeline : public TextPlotPipeline {
private:
    tcgfx::DeviceDrawable *drawable;
    Coord cursor;
public:
    explicit DrawableTextPlotPipeline(tcgfx::DeviceDrawable *drawable) : drawable(drawable) {}
    void drawPixel(uint16_t x, uint16_t y, uint32_t color) override {
        drawable->setDrawColor(color);
        drawable->drawPixel(x, y);
    }
    void setCursor(const Coord& where) override { cursor = where; }
    Coord getCursor() override { return cursor; }
    Coord getDimensions() override { return drawable->getDisplayDimensions(); }
};
#endif // GraphicsDevice available

#if __has_include (<U8g2lib.h>)
#include <U8g2lib.h>
#define UNICODE_U8G2_AVAILABLE
class U8g2TextPlotPipeline : public TextPlotPipeline {
private:
    U8G2* u8g2;
    Coord cursor;
public:
    explicit U8g2TextPlotPipeline(U8G2* gfx): u8g2(gfx) {}
    ~U8g2TextPlotPipeline() = default;
    void drawPixel(uint16_t x, uint16_t y, uint32_t color) override { u8g2->setColorIndex(color); u8g2->drawPixel(x, y); }
    Coord getDimensions() override {  return Coord(u8g2->getWidth(), u8g2->getHeight()); }
    void setCursor(const Coord& where) override { cursor = where; }
    Coord getCursor() override { return cursor; }
};
#endif // u8g2 included

#if __has_include (<Adafruit_GFX.h>)
#include <Adafruit_GFX.h>
#define UNICODE_ADAGFX_AVAILABLE
class AdafruitTextPlotPipeline : public TextPlotPipeline {
private:
    Adafruit_GFX *gfx;
public:
    explicit AdafruitTextPlotPipeline(Adafruit_GFX *gfx) : gfx(gfx) {
    }
    ~AdafruitTextPlotPipeline() = default;
    void drawPixel(uint16_t x, uint16_t y, uint32_t dc) override { return gfx->drawPixel(x, y, dc); }
    void setCursor(const Coord &where) override { gfx->setCursor(where.x, where.y); }
    Coord getCursor() override {return Coord(gfx->getCursorX(), gfx->getCursorY()); }
    Coord getDimensions() override { return Coord(gfx->width(), gfx->height());}
};
#endif // Adafruit included

#if __has_include (<TFT_eSPI.h>)
#include <TFT_eSPI.h>
#define UNICODE_TFT_ESPI_AVAILABLE
class TftSpiTextPlotPipeline : public TextPlotPipeline {
private:
    TFT_eSPI* tft;
    Coord cursor;
public:
    TftSpiTextPlotPipeline(TFT_eSPI* tft) : tft(tft) {}
    ~TftSpiTextPlotPipeline()=default;
    void drawPixel(uint16_t x, uint16_t y, uint32_t dc) override { return tft->drawPixel(x, y, dc); }
    Coord getDimensions() override { return Coord(tft->width(), tft->height());}
    void setCursor(const Coord& where) override { cursor = where; }
    Coord getCursor() override { return cursor; }
};
#endif // TFT_eSPI included

#define TC_UNICODE_CHAR_ERROR 0xffffffff

/**
 * Represents an item that can be drawn using the TcMenu font drawing functions. Regardless of if it is Adafruit
 * or TcUnicode we wrap it in one of these so the drawing code is always the same.
 */
class GlyphWithBitmap {
private:
    const uint8_t *bitmapData = nullptr;
    const UnicodeFontGlyph *glyph = nullptr;
public:
    /**
     * @return the actual bitmap data with offset already applied
     */
    const uint8_t *getBitmapData() const {
        return bitmapData;
    }

    /**
     * @return the glyph instructions for rendering. This structure is always in RAM and not progmem.
     */
    const UnicodeFontGlyph *getGlyph() const {
        return glyph;
    }

    void setBitmapData(const uint8_t *bm) {
        GlyphWithBitmap::bitmapData = bm;
    }

    void setGlyph(const UnicodeFontGlyph *g) {
        GlyphWithBitmap::glyph = g;
    }
};

void handleUtf8Drawing(void *userData, uint32_t ch);

#if __has_include (<Print.h>)
#include <Print.h>
class UnicodeFontHandler : public Print {
#elif __has_include(<PrintCompat.h>)
#include <PrintCompat.h>
class UnicodeFontHandler : public Print {
#else
class UnicodeFontHandler {
#endif

public:
    enum HandlerMode {
        HANDLER_SIZING_TEXT, HANDLER_DRAWING_TEXT
    };
private:
    tccore::Utf8TextProcessor utf8;
    TextPlotPipeline *plotter;
    union {
        const UnicodeFont *unicodeFont;
        const GFXfont *adaFont;
    };
    bool fontAdafruit = false;
    HandlerMode handlerMode = HANDLER_DRAWING_TEXT;
    uint16_t xExtentCurrent = 0;
    int16_t calculatedBaseline = -1;
    uint32_t drawColor = 0;
public:
    explicit UnicodeFontHandler(TextPlotPipeline *plotter, tccore::UnicodeEncodingMode mode) : utf8(handleUtf8Drawing, this, mode),
                                                                                               plotter(plotter),
                                                                                               unicodeFont(nullptr) {}
    virtual ~UnicodeFontHandler() = default;

#ifdef UNICODE_TCMENU_GRAPHIC_DEVICE_AVAILABLE
    explicit UnicodeFontHandler(DeviceDrawable *gfx, tccore::UnicodeEncodingMode mode) : utf8(handleUtf8Drawing, this, mode),
                                                                                         unicodeFont(nullptr) {
        plotter = new DrawableTextPlotPipeline(gfx);
    }
#endif

#ifdef UNICODE_ADAGFX_AVAILABLE
    explicit UnicodeFontHandler(Adafruit_GFX *gfx, tccore::UnicodeEncodingMode mode) : utf8(handleUtf8Drawing, this, mode),
                                                                                       unicodeFont(nullptr) {
        plotter = new AdafruitTextPlotPipeline(gfx);
    }
#endif

#ifdef UNICODE_TFT_ESPI_AVAILABLE
    explicit UnicodeFontHandler(TFT_eSPI* gfx, tccore::UnicodeEncodingMode mode): utf8(handleUtf8Drawing, this, mode), unicodeFont(nullptr) {
        plotter = new TftSpiTextPlotPipeline(gfx);
    }
#endif

#ifdef UNICODE_U8G2_AVAILABLE
    explicit UnicodeFontHandler(U8G2* gfx, tccore::UnicodeEncodingMode mode): utf8(handleUtf8Drawing, this, mode), unicodeFont(nullptr) {
        plotter = new U8g2TextPlotPipeline(gfx);
    }
#endif

    /**
     * Plotter pipelines allow the rendering of fonts to be customized to a greater extent, for example a transformation
     * pipeline could be added as a kind of passthrough.
     * @return  the current plotter
     */
    TextPlotPipeline *getTextPlotPipeline() { return plotter; }

    /**
     * Set a new pipeline as the means of plotting onto the display
     * @param newPipeline the new pipeline
     */
    void setTextPlotPipeline(TextPlotPipeline *newPipeline) { plotter = newPipeline; }

    /**
    * Sets the font to be a TcUnicode font
    * @param font a tcUnicode font
    */
    void setFont(const UnicodeFont *font) {
        unicodeFont = font;
        fontAdafruit = false;
        calculatedBaseline = -1;
    }

    /**
    * sets the font to be an Adafruit font
    * @param font an adafruit font
    */
    void setFont(const GFXfont *font) {
        adaFont = font;
        fontAdafruit = true;
        calculatedBaseline = -1;
    }

    /**
     * Set the cursor position where the next text will be drawn, depending on the library this may also change the
     * cursor for the library.
     * @param x the x position
     * @param y the y position
     */
    void setCursor(int16_t x, int16_t y) { plotter->setCursor(Coord(x, y)); }

    /**
     * Set the cursor position where the next text will be drawn, depending on the library this may also change the
     * cursor for the library.
     * @param where the new position
     */
    void setCursor(const Coord& where) { plotter->setCursor(where); }

    /**
    * Set the drawing color
    * @param color the new color for text drawing
    */
    void setDrawColor(uint32_t color) { this->drawColor = color; }

    /**
    * Prints a unicode character using the current font
    * @param unicodeChar the character to print.
    */
    void writeUnicode(uint32_t unicodeText);

    /**
    * Get the extents of the text provided in UTF8, optionally the string can be in program memory by providing the
    * third parameter as true.
    * @param text the text to get the length of, in UTF8
    * @param baseline the pointer to int for the baseline (amount below text), can be nullptr.
    * @param progMem optional, defaults to false, set to true for progMem.
    * @return the x and y extent of the text
    */
    Coord textExtents(const char *text, int *baseline, bool progMem = false);

    /**
    * Get the extents of the text provided in UTF8 from program memory.
    * @param text the UTF8 text in program memory to get the length of
    * @param baseline the pointer to int for the baseline (amount below text), can be nullptr.
    * @return the x and y extent of the text
    */
    Coord textExtents_P(const char *text, int *baseline) { return textExtents(text, baseline, true); }

#ifndef __MBED__

    /**
    * Get the extents of the text provided in UTF8 as a flash based string using the F() macro
    * @param fh the string provided using _F()
    * @param baseline the pointer to int for the baseline (amount below text), can be nullptr.
    * @return the X and Y extent of the text.
    */
    Coord textExtents(const __FlashStringHelper *fh, int *baseline) {
        return textExtents((const char *) fh, baseline, true);
    }

#endif

    /**
    * Get the extent of a single unicode character
    * @param theChar the unicode character to get the extent of
    * @return the x and y extent of the character
    */
    Coord textExtent(uint32_t theChar);

    /**
    * An extension to the print interface that allows printing from program memory somewhat easier
    * @param textPgm the text in program memory
    * @return the number of chars written
    */
    size_t print_P(const char *textPgm);

    /**
    * Get the absolute baseline of the font, this is the total height including any parts considered below
    * the yheight.
    * @return the baseline
    */
    int getBaseline();

    /**
    * Can be used to implement the print interface, the UTF8 implementation for this library is completely
    * asynchronous and keeps internal state, so it will wait until enough characters arrive to actually print
    * something.
    * @param data a byte of data in utf8
    */
    size_t write(uint8_t data) override;

    /**
     * Finds a character in the current font, if the character exists it will return true, and the referenced value
     * type (GlyphWithBitmap) will be filled in. Note that the returned glyph is always accessible without progmem
     * functions on any board. Whereas the bitmap will be in constant memory, so could we require progmem functions.
     * @param ch the character to find.
     * @param glyphBitmap a reference to a structure holding the Glyph and bitmap pointer. Only valid when true returned
     * @return true if successful, otherwise false.
     */
    bool findCharInFont(uint32_t ch, GlyphWithBitmap &glyphBitmap) const;
    /**
     * @return the total Y advance to move down a line. Call get baseline to get the amount below the baseline.
     */
    int getYAdvance() const {
        if(adaFont == nullptr) return 0;
        return pgm_read_byte((fontAdafruit ? &adaFont->yAdvance : &unicodeFont->yAdvance));
    }

    /**
     * Internal function called by the utf8 async callback
     * @param ch the unicode char
     */
    void internalHandleUnicodeFont(uint32_t ch);
};

using namespace tccore;

#endif //TC_UNICODE_FONTHELPER_H
