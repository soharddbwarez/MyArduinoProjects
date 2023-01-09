/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

#include "Utf8TextProcessor.h"

using namespace tccore;

void Utf8TextProcessor::reset() {
    // clear the text buffer
    extraCharsNeeded = 0;
    decoderState = WAITING_BYTE_0;
}

void Utf8TextProcessor::pushChars(const char *str) {
    while (*str) {
        pushChar(*str);
        str++;
    }
}

void Utf8TextProcessor::pushChar(char data) {
    if (encodingMode == ENCMODE_EXT_ASCII) {
        handler(userData, (uint8_t)data);
        return;
    }

    if((uint8_t)data == 0xFE || (uint8_t)data == 0xFF) {
        error(0); // should not be passed on ever
        return;
    }

    if (decoderState == WAITING_BYTE_0) {
        processChar0(data);
    } else if (decoderState == WAITING_BYTE_1) {
        if ((data & 0xc0) == 0x80) {
            uint16_t uni = data & 0x3F;
            if(extraCharsNeeded == 1) {
                currentUtfChar |= uni;
                decoderState = UTF_CHAR_FOUND;
            } else {
                int shiftAmount = extraCharsNeeded == 3 ? 12 : 6;
                currentUtfChar |= (uni << shiftAmount);
                decoderState = WAITING_BYTE_2;
            }
        } else {
            error(data);
        }
    } else if (decoderState == WAITING_BYTE_2) {
        if ((data & 0xc0) == 0x80) {
            uint16_t uni = data & 0x3F;
            if(extraCharsNeeded == 2) {
                decoderState = UTF_CHAR_FOUND;
                currentUtfChar |= uni;
            } else {
                currentUtfChar |= uni << 6;
                decoderState = WAITING_BYTE_3;
            }
        } else {
            error(data);
        }
    } else if (decoderState == WAITING_BYTE_3) {
        if ((data & 0xc0) == 0x80) {
            uint32_t uni = data & 0x3F;
            currentUtfChar |= uni;
            decoderState = UTF_CHAR_FOUND;
        } else {
            error(data);
        }
    }

    // we only go ahead and register the character when a character was found.
    if (decoderState == UTF_CHAR_FOUND) {
        decoderState = WAITING_BYTE_0;
        if(couldSequenceBeSmaller()) {
            error(0); // completely invalid sequence, ignore
        } else {
            handler(userData, currentUtfChar);
        }
    }
}

void Utf8TextProcessor::processChar0(char data) {
    if ((data & 0x80) == 0) {
        currentUtfChar = (uint8_t) data;
        extraCharsNeeded = 0;
        decoderState = UTF_CHAR_FOUND;
    } else if ((data & 0b11100000) == 0b11000000) {
        currentUtfChar = (uint32_t)(data & 0x1FU) << 6;
        decoderState = WAITING_BYTE_1;
        extraCharsNeeded = 1;
    } else if ((data & 0b11110000) == 0b11100000) {
        currentUtfChar = (uint32_t)(data & 0x0FU) << 12;
        decoderState = WAITING_BYTE_1;
        extraCharsNeeded = 2;
    } else if ((data & 0b11111000) == 0b11110000) {
        currentUtfChar = (uint32_t)(data & 0x07U) << 18;
        decoderState = WAITING_BYTE_1;
        extraCharsNeeded = 3;
    } else {
        error(data);
    }
}

void Utf8TextProcessor::error(char lastCode) {
    decoderState = WAITING_BYTE_0;
    extraCharsNeeded = 0;
    currentUtfChar = 0;
    handler(userData, TC_UNICODE_CHAR_ERROR);
    if(lastCode != 0) {
        processChar0(lastCode);
        if(decoderState == UTF_CHAR_FOUND) {
            decoderState = WAITING_BYTE_0;
            handler(userData, lastCode);
        }
    }
}

bool Utf8TextProcessor::couldSequenceBeSmaller() const {
    if(currentUtfChar < 0x80) {
        return extraCharsNeeded != 0;
    } else if(currentUtfChar < 0x800 ) {
        return extraCharsNeeded > 1;
    } else if(currentUtfChar < 0x10000) {
        return extraCharsNeeded > 2;
    }

    return false;
}