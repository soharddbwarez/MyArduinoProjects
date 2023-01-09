
#include <testing/SimpleTest.h>
#include <Fonts/OpenSansCyrillicLatin18.h>
#include <Fonts/RobotoMedium24.h>
#include <tcUnicodeHelper.h>
#include <SCCircularBuffer.h>
#include <IoLogging.h>

class UnitTestPlotter : public TextPlotPipeline {
private:
    GenericCircularBuffer<Coord> pixelsDrawn;
    uint32_t col = 0;
    Coord where = {0,0};
public:
    UnitTestPlotter(): pixelsDrawn(32) {}
    ~UnitTestPlotter() =default;

    void drawPixel(uint16_t x, uint16_t y, uint32_t color) override {
        pixelsDrawn.put(Coord(x, y));
    }

    void setCursor(const Coord &p) override {
        where = p;
    }

    Coord getCursor() override {
        return where;
    }

    Coord getDimensions() override {
        return Coord(320, 200);
    }

    void init() {
        while(pixelsDrawn.available()) pixelsDrawn.get();
        where = {0,0};
    }
} unitTestPlotter;

class TextUtilsFixture : public SimpleTest::UnitTestExecutor {
protected:
    UnicodeFontHandler* handler = nullptr;
public:
    void setup() override {
        unitTestPlotter.init();
        handler = new UnicodeFontHandler(&unitTestPlotter, ENCMODE_UTF8);
        handler->setFont(OpenSansCyrillicLatin18);
        handler->setDrawColor(20);
    }

    void teardown() override {
        delete handler;
    }

    bool checkGlyph(uint32_t code, uint32_t bmpOffset, int width, int height, int xAdvance, int xOffset, int yOffset) {
        GlyphWithBitmap glyphWithBitmap;
        if(handler->findCharInFont(code, glyphWithBitmap)) {
            const UnicodeFontGlyph *glyph = glyphWithBitmap.getGlyph();
            bool success = true;
            if(bmpOffset != glyph->relativeBmpOffset) {
                serlogF4(SER_DEBUG, "Bmp offset out ", code, bmpOffset, glyph->relativeBmpOffset);
                success = false;
            }
            if(width != glyph->width || height != glyph->height) {
                serlogF4(SER_DEBUG, "Bmp width out ", code, width, glyph->width);
                serlogF3(SER_DEBUG, "Bmp height ", height, glyph->height);
                success = false;
            }
            if(xOffset != glyph->xOffset || yOffset != glyph->yOffset) {
                serlogF4(SER_DEBUG, "Bmp xoffs out ", code, xOffset, glyph->xOffset);
                serlogF3(SER_DEBUG, "Bmp yoffs ", yOffset, glyph->yOffset);
                success = false;
            }
            if(xAdvance != glyph->xAdvance) {
                serlogF4(SER_DEBUG, "Bmp xadv out ", code, xAdvance, glyph->xAdvance);
                success = false;
            }

            return success;
        } else {
            serlogF2(SER_DEBUG, "Glyph not found ", code);
            return false;
        }
        
    }
};

testF(TextUtilsFixture, testTextExtents) {
    int bl;
    handler->setFont(OpenSansCyrillicLatin18);
    Coord coord = handler->textExtents("Abc", &bl, false);
    assertEquals((int16_t)31, coord.x);
    assertEquals((int16_t)24, coord.y);

    handler->setFont(&RobotoMedium24);
    coord = handler->textExtents("Abc", &bl, false);
    assertEquals((int16_t)43, coord.x);
    assertEquals((int16_t)28, coord.y);
}

testF(TextUtilsFixture, testGetGlyphOnEachRange) {
    assertTrue(checkGlyph(65, 320, 11, 13, 11, 0, -18));
    assertTrue(checkGlyph(55 + 128, 198, 2, 2, 5, 1, -12));
    assertTrue(checkGlyph(17 + 1024, 285, 8, 13, 11, 2, -18));
}

testF(TextUtilsFixture, testReadingEveryGlyphInRange) {
    GlyphWithBitmap glyphWithBitmap;

    // test all known characters work
    for(int i=32;i<127;i++) {
        serlogF2(SER_DEBUG, "Test character = ", i);
        assertTrue(handler->findCharInFont(i, glyphWithBitmap));
        assertNotEquals(nullptr, glyphWithBitmap.getGlyph());
        assertNotEquals(nullptr, glyphWithBitmap.getBitmapData());
    }

    // test a few that should fail
    assertFalse(handler->findCharInFont(0, glyphWithBitmap));
    assertFalse(handler->findCharInFont(5, glyphWithBitmap));
    assertFalse(handler->findCharInFont(10, glyphWithBitmap));
    assertFalse(handler->findCharInFont(0xFFFF, glyphWithBitmap));
}

testF(TextUtilsFixture, testAdafruitFont) {
    GlyphWithBitmap glyphWithBitmap;
    handler->setFont(&RobotoMedium24);

    // test a few that should fail
    assertTrue(checkGlyph(32, 0, 0, 0, 6, 0, -28));
    assertTrue(checkGlyph(48, 243, 12, 17, 14, 1, -23));
    assertTrue(checkGlyph(65, 611, 16, 17, 16, 0, -23));
    assertTrue(checkGlyph(126, 1990, 14, 5, 16, 1, -16));
    assertFalse(handler->findCharInFont(5, glyphWithBitmap));
    assertFalse(handler->findCharInFont(127, glyphWithBitmap));
    assertFalse(handler->findCharInFont(31, glyphWithBitmap));

}