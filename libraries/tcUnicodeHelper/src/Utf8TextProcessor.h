/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#ifndef TCMENU_UTF8TEXTPROCESSOR_H
#define TCMENU_UTF8TEXTPROCESSOR_H

#include <string.h>
#include <inttypes.h>

/**
 * @file Utf8TextProcessor.h
 * @brief contains a strict asynchronous UTF-8 decoder that uses very little memory.
 */

/* the value that defines an error in character conversion, usual procedure is to call reset() on the text processor */
#define TC_UNICODE_CHAR_ERROR 0xffffffff

namespace tccore {

    /**
     * Tells the UTF8 decoder which mode it is in, either extended ASCII or UTF8
     */
    enum UnicodeEncodingMode { ENCMODE_UTF8, ENCMODE_EXT_ASCII };

    /**
     * This callback will be called for every converted UTF8 or ASCII sequence (depending on the mode of the handler).
     * You'll get the void pointer you provided during construction, and the characeter that was converted.
     */
    typedef void (*UnicodeCharacterHandler)(void* callbackData, uint32_t convertedChar);

    /**
     * A completely asynchronous implementation of a UTF8 encoder. Can work with the Print interface as it expects no
     * end to the stream, and requires only a very simple change in the write function.
     */
    class Utf8TextProcessor {
    public:
        enum DecoderState {
            WAITING_BYTE_0, WAITING_BYTE_1, WAITING_BYTE_2, WAITING_BYTE_3, UTF_CHAR_FOUND
        };
    private:
        DecoderState decoderState = WAITING_BYTE_0;
        uint32_t currentUtfChar = 0U;
        int extraCharsNeeded = 0;
        UnicodeCharacterHandler handler;
        void* userData;
        const UnicodeEncodingMode encodingMode;
    public:
        /**
         * Create an instance of a UTF-8 decoder that will asynchronous process data calling the handler as characters
         * are decoded from the stream.
         * @param handler the callback that receives characters
         * @param userData an optional piece of data to pass back to you in the callback, can be nullptr
         * @param mode processing mode, either ENCMODE_UTF8 or ENCMODE_EXT_ASCII
         */
        explicit Utf8TextProcessor(UnicodeCharacterHandler handler, void* userData, UnicodeEncodingMode mode)
                : handler(handler), userData(userData), encodingMode(mode) {}

        /** completely reset the decoder back to initial state */
        void reset();

        /**
         * Pushes a single character through the decoder. As characters are decoded the callback will fire.
         * @param ch the character to process
         */
        void pushChar(char ch);

        /**
         * Pushes a string of character data through the encoder.  As characters are decoded the callback will fire.
         * @param str the string of data to process
         */
        void pushChars(const char *str);

    private:
        /**
         * Indicates an error has occurred, somewhat internal to the push.. functions
         * @param lastCode the last character that we received.
         */
        void error(char lastCode);
        /**
         * Check if the sequence could be smaller, this is to ensure overlong sequences are not allowed
         * @return true if the sequence could be smaller, otherwise false
         */
        bool couldSequenceBeSmaller() const;

        /**
         * When this is called, we are processing the first character in a UTF-8 sequence.
         * @param data the first character of data.
         */
        void processChar0(char data);
    };

}



#endif //TCMENU_UTF8TEXTPROCESSOR_H
