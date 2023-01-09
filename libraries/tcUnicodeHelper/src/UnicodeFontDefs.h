/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#ifndef TCMENU_UNICODE_FONT_DEFN
#define TCMENU_UNICODE_FONT_DEFN

#include <inttypes.h>

#ifdef HUGE_FONT_BITMAPS
typedef uint32_t bitmap_size_t;
#else
typedef uint16_t bitmap_size_t;
#endif //HUGE_FONT_BITMAPS

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/**
 * @file UnicodeFontDefs.h
 * @brief contains font rendering structures both for AdafruitGFX fonts and TcUnicode fonts
 */

/*
 * The next section is copied from Adafruit_GFX, it provides support for using Adafruit GFX fonts with this library.
 *
 * Font structures for newer Adafruit_GFX (1.1 and later).
 * Example fonts are included in 'Fonts' directory.
 * To use a font in your Arduino sketch, #include the corresponding .h
 * file and pass address of GFXfont struct to setFont().
 */

/**
 * The adafruit font glyph format, each letter in the font is represented by a GFXglyph in an array. This indicates how
 * to read the bitmap and process it, the width and height are related to the bitmap, but the bitmap may not take up
 * the full area, the xAdvance is how far to move in the x dimension after rendering the bitmap, the x and y offsets
 * tell the renderer where to start drawing the bit. With these fonts, the y start point is always the baseline.
 */
typedef struct {
    uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
    uint8_t width;         ///< Bitmap dimensions in pixels
    uint8_t height;        ///< Bitmap dimensions in pixels
    uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
    int8_t xOffset;        ///< X dist from cursor pos to UL corner
    int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/**
 * This represents an adafruit font, it holds the bitmap data for all glyphs, and also includes and array of glyphs,
 * these are sequential characters from first through to last. Lastly, the yAdvance indicates how far to go for a new
 * line.
 */
typedef struct {
    uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
    GFXglyph *glyph;  ///< Glyph array
    uint16_t first;   ///< ASCII extents (first char)
    uint16_t last;    ///< ASCII extents (last char)
    uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;

#endif // GFXFont include

/**
 * Indicates how to read the pixel map within the font. In future I plan to support at least four greyscale levels which
 * allows basic anti-aliasing, and if thre is demand fonts with multiple palette colours, but at the moment only one bit
 * per pixel is supported.
 */
enum BitmapFormat: uint8_t { TCFONT_ONE_BIT_PER_PIXEL };

/**
 * The TcUnicode glyph format is very similar to the adafruit glyph format, other than some small differences to make
 * unicode handling easier. The biggest difference is the relativeChar support, so it is possible to skip parts of a
 * font that are not needed. This char code is relative to the unicode block. The bit map offset has been adjusted to
 * 17 bits, to allow each block to have 128K of bitmaps.
 *
 * This glyph data indicates how to read the bitmap and process it, the width and height are related to the bitmap,
 * but the bitmap may not take up the full area, the xAdvance is how far to move in the x dimension after rendering the
 * bitmap, the x and y offsets tell the renderer where to start drawing the bit. With these fonts, the y start point is
 * always the baseline.
 */
typedef struct {
    /** The character number in the array */
    uint16_t relativeChar;
    /** offset into the bitmap array */
    bitmap_size_t relativeBmpOffset;
    /** width of the bitmap in pixels */
    uint8_t width;
    /** height of the bitmap in pixels */
    uint8_t height;
    /** how far to advance in the x dimension */
    uint8_t xAdvance;
    /** the x offset from the UL corner */
    int8_t xOffset;
    /** the y offset from the UL corner, usually negative */
    int8_t yOffset;
} UnicodeFontGlyph;

/**
 * A unicode block is a range of character akin to a code page. In this structure we store the bitmaps and glyphs that
 * are associated with the block range. This allows us to be more efficient with memory overall. The char nums in a glyph
 * are actually calculated by startingId + charNum
 */
typedef struct {
    /** The starting point for this block, all glyph entries are an offset from this */
    uint32_t startingNum;
    /** the array of bitmaps for each letter */
    const uint8_t *bitmap;
    /** The glyphs within this block */
    const UnicodeFontGlyph *glyphs;
    /** The number of points in this block */
    uint16_t numberOfPoints;
} UnicodeFontBlock;

/**
 * This represents the whole font, and is passed to an TcUnicode renderer requiring a font. It is basically an array
 * of blocks sorted by starting code in reverse order. It also has instructions on how to process the bitmap structure
 * for future improvements, and the yAdvance.
 */
typedef struct {
    /** the array of unicode glyphs */
    const UnicodeFontBlock *unicodeBlocks;
    /** the number of items */
    uint16_t numberOfBlocks;
    /** the height of each line */
    uint8_t yAdvance;
    BitmapFormat bitmapFormat;
} UnicodeFont;

#endif // TCMENU_UNICODE_FONT_DEFN
