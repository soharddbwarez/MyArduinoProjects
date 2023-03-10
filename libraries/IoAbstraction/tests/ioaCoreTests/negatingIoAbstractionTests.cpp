#include <TaskManagerIO.h>
#include <testing/SimpleTest.h>
#include "MockIoAbstraction.h"
#include "NegatingIoAbstraction.h"

using namespace SimpleTest;

test(testNegatingIoAbstractionRead) {
    MockedIoAbstraction mockIo;
    NegatingIoAbstraction negatingIo(&mockIo);

    for(int i=0;i<8;i++) {
        negatingIo.pinDirection(i, INPUT);
        negatingIo.pinDirection(i+8, OUTPUT);
    }

    // set the upfront read values
    mockIo.setValueForReading(0, 0xff);
    mockIo.setValueForReading(1, 0x00);

    // and then check that the read is inverting as expected.
    assertTrue(ioDeviceDigitalRead(&mockIo, 0));
    assertFalse(ioDeviceDigitalRead(&negatingIo, 0));
    // also check the sync is working.
    ioDeviceSync(&negatingIo);
    assertFalse(ioDeviceDigitalRead(&mockIo, 0));
    assertTrue(ioDeviceDigitalRead(&negatingIo, 0));

    // check that port reads are inverted
    assertEquals(0x00U, (unsigned int)ioDeviceDigitalReadPort(&mockIo, 0));
    assertEquals(0xffU, (unsigned int)ioDeviceDigitalReadPort(&negatingIo, 0));

    mockIo.resetIo();

    // now test writing to the port is inverted.
    ioDeviceDigitalWrite(&negatingIo, 9, HIGH);
    assertEquals(mockIo.getWrittenValue(0), (uint16_t)0);
    ioDeviceDigitalWrite(&negatingIo, 9, LOW);
    assertEquals(mockIo.getWrittenValue(0), (uint16_t)0x200U);

    ioDeviceDigitalWritePort(&negatingIo, 9, 0x00);
    int toCompare = mockIo.getWrittenValue(0) >> 8;
    assertEquals(toCompare, 0xff);

    // lastly make sure there were no IO errors.
    assertEquals(mockIo.getErrorMode(), NO_ERROR);
}

