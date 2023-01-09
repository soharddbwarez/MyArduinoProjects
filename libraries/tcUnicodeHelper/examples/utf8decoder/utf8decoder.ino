
#include <Utf8TextProcessor.h>
#include <Wire.h>

using namespace tccore;

//
// Each time a character is decoded by the utf8 processor, it will call back this function. The user data provided
// during construction is also provided back to you with the converted code, if there is an error in the stream the
// library recovers automatically after calling back with TC_UNICODE_CHAR_ERROR as the code.
//
void characterReceived(void* optionalUserData, uint32_t convertedCode) {
    Serial1.print("Unicode=");
    Serial1.println(convertedCode);
}

//
// here we register a utf8 processor that takes in characters and outputs unicode codes. It is asynchronous in that
// you provide it data then as and when output is available it calls the callback.
//
void* optionalUserData = nullptr;
Utf8TextProcessor textProcessor(characterReceived, optionalUserData, ENCMODE_UTF8); // ENCMODE_EXT_ASCII for extended ASCII processing mode

void setup() {
    Serial1.begin(115200);

    // put characters into the decoder. Both ASCII and Unicode
    textProcessor.pushChars("Hello world");
    textProcessor.pushChars("Привіт Світ");

    // you can push things in a char at a time, even UTF8 unicode encoded data, it will decode it as it arrives
    textProcessor.pushChar('A');

    // should you wish to, you can also reset the decoder back to its initial state.
    textProcessor.reset();
}

void loop() {
    delay(1000);
}