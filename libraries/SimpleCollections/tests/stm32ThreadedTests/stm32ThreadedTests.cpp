
#include <Arduino.h>
#include <testing/SimpleTest.h>
#include <Wire.h>
#include <HardwareTimer.h>
#include <SCCircularBuffer.h>

using namespace SimpleTest;

HardwareTimer* myTimer;

struct WriterStruct {
    uint32_t sequence;
    uint32_t data1;
    uint32_t data2;

    void setData(uint32_t s, uint32_t d1, uint32_t d2) {
        sequence = s;
        data1 = d1;
        data2 = d2;
    }
};

//
// The only safe way to work with multibyte objects is to have a pool of them the same size as the buffer itself.
// We test this use case here. Many threads writing often may not be a good use case for spin locking, it's going to
// spin an awful lot of the time, and proper semaphore/mutex construct *may* be better when there are many writers.
//
GenericCircularBuffer<WriterStruct> writerMemoryAlloc(50, GenericCircularBuffer<WriterStruct>::MEMORY_POOL);
GenericCircularBuffer<WriterStruct*> actualBuffer(50);

uint32_t nextSequence=0;

void timerHasChanged() {
    auto &alloc = writerMemoryAlloc.get();
    alloc.setData(nextSequence, nextSequence * 1000, nextSequence * 2000);
    actualBuffer.put(&alloc);
    nextSequence++;
}

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("Start");
    startTesting();

#if defined(TIM1)
    TIM_TypeDef *Instance = TIM1;
#else
    TIM_TypeDef *Instance = TIM2;
#endif

    // Instantiate HardwareTimer object. Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
    myTimer = new HardwareTimer(Instance);
    pinMode(LED_BUILTIN, OUTPUT);
    myTimer->setOverflow(2000, HERTZ_FORMAT);
    myTimer->attachInterrupt(timerHasChanged);
}

DEFAULT_TEST_RUNLOOP

test(testThreadedWriterAndReaderByte) {
    nextSequence = 0;
    myTimer->resume();

    auto millisThen = millis();
    int itemsReceived = 0;
    while(itemsReceived < 20000U && (millis() - millisThen) < 10000) {
        if (actualBuffer.available()) {
            auto data = actualBuffer.get();
            // don't normally include the below debug, the test will fail, but useful for diagnostics.
            //serdebugF4("Thread read ", data->sequence, data->data1, data->data2);
            assertEquals(itemsReceived, (int)data->sequence);
            itemsReceived++;
        }
    }

    assertMoreThan((uint8_t)19000, itemsReceived);

    myTimer->pause();
}
