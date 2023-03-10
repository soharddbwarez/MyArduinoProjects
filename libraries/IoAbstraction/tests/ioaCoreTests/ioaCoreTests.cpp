
#include <testing/SimpleTest.h>
#include <IoAbstraction.h>
#include <PlatformDeterminationWire.h>
#include <MockEepromAbstraction.h>
#include <EepromAbstractionWire.h>

using namespace SimpleTest;

char memToWrite[110] = { };
char readBuffer[110] = { };

bool romClear(EepromAbstraction& eeprom, EepromPosition pos) {
    for(int i=0;i<100;i++) {
        eeprom.write8(pos + i, 0xaa);

        if(eeprom.read8(pos + i) != 0xaa) return false;
    }
    return true;
}

// for devices where an I2C rom is not installed. We cannot directly test.
#ifndef IOA_EXCLUDE_I2C_TESTS

test(testI2cArrayWrites) {
    I2cAt24Eeprom eeprom(0x50, PAGESIZE_AT24C128);
    assertTrue(romClear(eeprom, 700));
    serdebug("Run array tests on i2c rom");

    strcpy(memToWrite, "This is a very large string to write into the rom to ensure it crosses memory boundaries in the rom");
    eeprom.writeArrayToRom(710, (const uint8_t*)memToWrite, sizeof(memToWrite));

    serdebug("I2C eeprom array written.");

    eeprom.readIntoMemArray((uint8_t*)readBuffer, 710, sizeof(memToWrite));
    assertStringEquals(memToWrite, readBuffer);
    serdebug("Read into mem done");
}

test(testI2cSingleWrites) {
    serdebug("Run single tests on i2c rom");
    I2cAt24Eeprom eeprom(0x50, PAGESIZE_AT24C128);
    assertTrue(romClear(eeprom, 700));

    eeprom.write8(700, 0xFF);
    assertEquals(0xFF, eeprom.read8(700));
    eeprom.write8(700, 0xDD);
    assertEquals(0xDD, eeprom.read8(700));

    eeprom.write16(701, 0xf00d);
    eeprom.write32(703, 0xbeeff00d);

    yield();
    serdebug("I2C reads...");

    assertEquals((uint16_t)0xf00d, eeprom.read16(701));
    assertEquals((uint32_t)0xbeeff00d, eeprom.read32(703));

    assertFalse(eeprom.hasErrorOccurred());
}

test(badI2cEepromDoesNotLockCode) {
    serdebug("I2C bad EEPROM address test start.");

    I2cAt24Eeprom eepromBad(0x73, PAGESIZE_AT24C128);
    eepromBad.write8(800, 123);
    assertTrue(eepromBad.hasErrorOccurred());

    serdebug("I2C bad EEPROM address test end.");
}

#endif

test(testMockEeprom) {
    MockEepromAbstraction eeprom(256);
    eeprom.write8(0, 0xfe);
    eeprom.write16(1, 0xf00d);
    eeprom.write32(3, 0xbeeff00d);
    eeprom.writeArrayToRom(10, (const uint8_t*)memToWrite, sizeof(memToWrite));

    assertEquals((uint8_t)0xfe, eeprom.read8(0));
    assertEquals((uint16_t)0xf00d, eeprom.read16(1));
    assertEquals((uint32_t)0xbeeff00d, eeprom.read32(3));
    eeprom.readIntoMemArray((uint8_t*)readBuffer, 10, sizeof(memToWrite));
    assertStringEquals(memToWrite, readBuffer);

    // now try other values to ensure the prior test worked
    eeprom.write8(0, 0xaa);
    assertEquals((uint8_t)0xaa, eeprom.read8(0));
    assertFalse(eeprom.hasErrorOccurred());

    // write beyond boundary
    eeprom.write16(1000, 0xbad);
    assertTrue(eeprom.hasErrorOccurred());
}

IOLOG_MBED_PORT_IF_NEEDED(USBTX, USBRX)

#ifdef IOA_USE_MBED
I2C Wire(PF_0, PF_1);
#endif

void setup() {
    IOLOG_START_SERIAL

#ifdef IOA_USE_MBED
    ioaWireBegin(&Wire);
#else
    Wire.begin();
#endif
    startTesting();
}

DEFAULT_TEST_RUNLOOP
