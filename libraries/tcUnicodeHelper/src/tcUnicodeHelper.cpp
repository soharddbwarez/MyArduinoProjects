/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#include "tcUnicodeHelper.h"

Coord UnicodeFontHandler::textExtents(const char *text, int *baseline, bool progMem) {
    if(adaFont == nullptr) {
        baseline = 0;
        return Coord(0,0);
    }

    handlerMode = HANDLER_SIZING_TEXT;
    xExtentCurrent = 0;
    utf8.reset();
    if(progMem) {
        uint8_t c;
        while ((c = pgm_read_byte(text++))) utf8.pushChar((char)c);
    } else {
        utf8.pushChars(text);
    }
    handlerMode = HANDLER_DRAWING_TEXT;

    if(baseline) {
        *baseline = getBaseline();
    }
    return Coord((int)xExtentCurrent, getYAdvance());
}

#define internal_max(a, b)  ((a) < (b) ? (b) : (a));

void UnicodeFontHandler::writeUnicode(uint32_t unicodeText) {
    // make sure it's printable.
    auto dims = plotter->getDimensions();
    auto posn = plotter->getCursor();
    if (posn.x > (int32_t) dims.x) return;

    GlyphWithBitmap gb;
    if(!findCharInFont(unicodeText, gb)) return;
    uint8_t w = gb.getGlyph()->width, h = gb.getGlyph()->height;
    int8_t xo = gb.getGlyph()->xOffset, yo = gb.getGlyph()->yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;

    uint16_t xDim = dims.x;
    uint16_t yDim = dims.y;
    uint16_t x = posn.x;
    uint16_t y = posn.y;

    const uint8_t* bitmap = gb.getBitmapData();
    int bo = 0;
    for (yy = 0; yy < h; yy++) {
        auto locY = internal_max(0, y + yo + yy);
        bool yOK = (locY < yDim);
        for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) {
                bits = pgm_read_byte(&bitmap[bo++]);
            }
            if (bits & 0x80) {
                int locX = internal_max(0, int(x +xo + xx));
                if (locX < xDim && yOK) {
                    plotter->drawPixel(locX, locY, drawColor);
                }
            }
            bits <<= 1;
        }
    }
    plotter->setCursor(Coord(posn.x + gb.getGlyph()->xAdvance, posn.y));
}

Coord UnicodeFontHandler::textExtent(uint32_t theChar) {
    GlyphWithBitmap gb;
    if (!findCharInFont(theChar, gb)) {
        return Coord(0, getYAdvance());
    }
    return Coord(gb.getGlyph()->xAdvance, getYAdvance());
}

const UnicodeFontGlyph *findWithinGlyphs(const UnicodeFontBlock* block, uint32_t ch) {
    size_t start = 0;
    size_t end = pgm_read_word(&block->numberOfPoints) - 1;
    bool failed = false;
    auto glyphs = (const UnicodeFontGlyph *)pgm_read_ptr(&block->glyphs);
    while (!failed) {
        if(pgm_read_word(&glyphs[start].relativeChar) == ch) return &glyphs[start];
        if(pgm_read_word(&glyphs[end].relativeChar) == ch) return &glyphs[end];

        size_t middle = ((end - start) / 2) + start;
        uint32_t charNumMiddle = pgm_read_word(&glyphs[middle].relativeChar);
        if (charNumMiddle == ch) return &glyphs[middle];
        if (ch < charNumMiddle) {
            end = middle;
        } else {
            start = middle;
        }
        if ((end - start) < 2) failed = true;
    }
    return nullptr;
}

UnicodeFontGlyph globalGlyphForFindChar;

inline void copyFontGlyphFromProgmem(UnicodeFontGlyph* dest, const UnicodeFontGlyph* src) {
    memcpy_P(dest, src, sizeof(UnicodeFontGlyph));
}

bool UnicodeFontHandler::findCharInFont(uint32_t code, GlyphWithBitmap& glyphBitmap) const {
    if(adaFont == nullptr) return false; // no font, then no characters!

    if (fontAdafruit) {
        auto firstCode = pgm_read_word(&adaFont->first);
        if (code < firstCode || code > pgm_read_word(&adaFont->last)) return false;
        uint32_t idx = code - firstCode;
        auto glyphs = (const GFXglyph*)pgm_read_ptr(&adaFont->glyph);
        globalGlyphForFindChar.relativeChar = code;
        globalGlyphForFindChar.height = pgm_read_byte(&glyphs[idx].height);
        globalGlyphForFindChar.width = pgm_read_byte(&glyphs[idx].width);
        globalGlyphForFindChar.xAdvance = pgm_read_byte(&glyphs[idx].xAdvance);
        globalGlyphForFindChar.xOffset = (int8_t)pgm_read_byte(&glyphs[idx].xOffset);
        globalGlyphForFindChar.yOffset = (int8_t)pgm_read_byte(&glyphs[idx].yOffset);
        globalGlyphForFindChar.relativeBmpOffset = pgm_read_word(&glyphs[idx].bitmapOffset); // unused
        glyphBitmap.setGlyph(&globalGlyphForFindChar);
        uint8_t* bitmapPtr = ((uint8_t*)pgm_read_ptr(&adaFont->bitmap)) + globalGlyphForFindChar.relativeBmpOffset;
        glyphBitmap.setBitmapData(bitmapPtr);
        return true;
    } else {
        uint16_t numBlocks = pgm_read_word(&unicodeFont->numberOfBlocks);
        auto blocks = (const UnicodeFontBlock*) pgm_read_ptr(&unicodeFont->unicodeBlocks);
        for (uint16_t i = 0; i < numBlocks; i++) {
            uint32_t startingNum = pgm_read_dword(&blocks[i].startingNum);
            uint32_t endingNum = startingNum + pgm_read_word(&blocks[i].numberOfPoints);
            if (code >= startingNum && code <= endingNum) {
                const UnicodeFontGlyph *glyph = findWithinGlyphs(&blocks[i], code - startingNum);
                if (glyph != nullptr) {
                    copyFontGlyphFromProgmem(&globalGlyphForFindChar, glyph);
                    glyphBitmap.setGlyph(&globalGlyphForFindChar);
                    glyphBitmap.setBitmapData(((uint8_t*)pgm_read_ptr(&blocks[i].bitmap)) + globalGlyphForFindChar.relativeBmpOffset);
                    return true;
                }
            }
        }
    }

    return false;
}

void UnicodeFontHandler::internalHandleUnicodeFont(uint32_t ch) {
    if (ch == TC_UNICODE_CHAR_ERROR) {
        utf8.reset();
        return;
    }

    switch(handlerMode) {
        case HANDLER_SIZING_TEXT:
            xExtentCurrent += textExtent(ch).x;
            break;
        case HANDLER_DRAWING_TEXT:
            writeUnicode(ch);
            break;
    }
}

size_t UnicodeFontHandler::write(uint8_t data) {
    if(adaFont == nullptr) return 0;

    handlerMode = HANDLER_DRAWING_TEXT;
    utf8.pushChar((char)data);
    return 1;
}

size_t UnicodeFontHandler::print_P(const char *textPgm) {
    if(adaFont == nullptr) return 0;
    uint8_t c;
    size_t count= 0;
    while ((c = pgm_read_byte(textPgm++))) {
        utf8.pushChar((char)c);
        count++;
    }
    return count;
}

int UnicodeFontHandler::getBaseline() {
    if(adaFont == nullptr) return 0;
    if(calculatedBaseline == -1) {
        auto current = "|jy";
        int bl = 0;
        while (*current) {
            GlyphWithBitmap gb;
            if (findCharInFont(*current, gb)) {
                bl += (gb.getGlyph()->height + gb.getGlyph()->yOffset);
            }
            current++;
        }
        calculatedBaseline = bl / 3;
    }
    return calculatedBaseline;
}

void handleUtf8Drawing(void *data, uint32_t ch) {
    auto fontHandler = reinterpret_cast<UnicodeFontHandler *>(data);
    fontHandler->internalHandleUnicodeFont(ch);
}
