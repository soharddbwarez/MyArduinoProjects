// with thanks to https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt for many of the ideas in this test.

#include <SimpleCollections.h>
#include <SCCircularBuffer.h>
#include <Utf8TextProcessor.h>
#include <testing/SimpleTest.h>

using namespace SimpleTest;
using namespace tccore;

IOLOG_MBED_PORT_IF_NEEDED(USBTX, USBRX)

void setup() {
    IOLOG_START_SERIAL
    startTesting();
}

DEFAULT_TEST_RUNLOOP

GenericCircularBuffer<uint32_t> unicodeChars(10);

void textHandler(void* handler, uint32_t charCode) {
    unicodeChars.put(charCode);
}

uint32_t getFromBufferOrError() {
    if(unicodeChars.available()) {
        return unicodeChars.get();
    } else {
        return TC_UNICODE_CHAR_ERROR;
    }
}

test(testUtf8EncoderAscii) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, ENCMODE_EXT_ASCII);
    textProcessor.pushChars("Hello");
    // force extended ascii processing!
    textProcessor.pushChar((char)199);
    textProcessor.pushChar((char)200);

    assertEquals((uint32_t)72, getFromBufferOrError());
    assertEquals((uint32_t)101, getFromBufferOrError());
    assertEquals((uint32_t)108, getFromBufferOrError());
    assertEquals((uint32_t)108, getFromBufferOrError());
    assertEquals((uint32_t)111, getFromBufferOrError());
    assertEquals((uint32_t)199U, getFromBufferOrError());
    assertEquals((uint32_t)200U, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testUtf8EncoderUnicodeCodesDirect) {
    while(unicodeChars.available()) unicodeChars.get();
    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);

    textProcessor.pushChar((char)0b00100100);
    assertEquals((uint32_t)0x24, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.pushChar((char)0b11000010);
    textProcessor.pushChar((char)0b10100011);
    assertEquals((uint32_t)0xA3, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.pushChar((char)0b11100010);
    textProcessor.pushChar((char)0b10000010);
    textProcessor.pushChar((char)0b10101100);
    assertEquals((uint32_t)0x20AC, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}


test(testUtf8EncoderUnicodeBasicCase) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChars("Hello");

    assertEquals((uint32_t)72, getFromBufferOrError());
    assertEquals((uint32_t)101, getFromBufferOrError());
    assertEquals((uint32_t)108, getFromBufferOrError());
    assertEquals((uint32_t)108, getFromBufferOrError());
    assertEquals((uint32_t)111, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testUtf8EncoderUnicodeMulti) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChars("Світ");// \xD0\xA1\xD0\xB2\xD1\x96\xD1\x82

    assertEquals((uint32_t)0x0421, getFromBufferOrError());
    assertEquals((uint32_t)0x0432, getFromBufferOrError());
    assertEquals((uint32_t)0x0456, getFromBufferOrError());
    assertEquals((uint32_t)0x0442, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testUtf8EncoderUnicodeOverlongNull) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChar((char)0xc0);
    textProcessor.pushChar((char)0x80);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xe0);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0x80);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xf0);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0x80);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testBrokenSequenceContinueAsAscii) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChar((char)0xc3);
    textProcessor.pushChar((char)'H');
    textProcessor.pushChar((char)'I');
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertEquals((uint32_t)'H', getFromBufferOrError());
    assertEquals((uint32_t)'I', getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xe1);
    textProcessor.pushChar((char)0x84);
    textProcessor.pushChar((char)'H');
    textProcessor.pushChar((char)'I');
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertEquals((uint32_t)'H', getFromBufferOrError());
    assertEquals((uint32_t)'I', getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xf1);
    textProcessor.pushChar((char)0x82);
    textProcessor.pushChar((char)0x81);
    textProcessor.pushChar((char)'A');
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertEquals((uint32_t)'A', getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xf1);
    textProcessor.pushChars("С");// Cyrillic letter
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertEquals((uint32_t)0x0421, getFromBufferOrError());

    textProcessor.reset();
    textProcessor.pushChar((char)0xf1);
    textProcessor.pushChar((char)0x81);
    textProcessor.pushChars("С");// Cyrillic letter
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertEquals((uint32_t)0x0421, getFromBufferOrError());
}

test(invalidBytesNotProcessed) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChar((char)0xff);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xfe);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testUtf8EncoderUnicodeOverlongSlash) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChar((char)0xc0);
    textProcessor.pushChar((char)0xAF);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xe0);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0xAF);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());

    textProcessor.reset();
    textProcessor.pushChar((char)0xf0);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0x80);
    textProcessor.pushChar((char)0xAF);
    assertEquals((uint32_t)TC_UNICODE_CHAR_ERROR, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}

test(testUtf8EncoderReallyLargeCodes) {
    while(unicodeChars.available()) unicodeChars.get();

    Utf8TextProcessor textProcessor(textHandler, nullptr, tccore::ENCMODE_UTF8);
    textProcessor.pushChars("ﬓﬔﬕﬖ");
    // force extended ascii processing!

    assertEquals((uint32_t)0xFB13, getFromBufferOrError());
    assertEquals((uint32_t)0xFB14, getFromBufferOrError());
    assertEquals((uint32_t)0xFB15, getFromBufferOrError());
    assertEquals((uint32_t)0xFB16, getFromBufferOrError());
    assertFalse(unicodeChars.available());
}