/**
 * This example demonstrates the analog abstraction that can use the same interface to
 * work with various analog devices such as DAC, ADC, PWM and POTs wherever they are
 * located. As more devices are added it will expand the possibilities of where This
 * common interface can be used.
 *
 * Documentation and reference:
 *
 * https://www.thecoderscorner.com/products/arduino-downloads/io-abstraction/
 * https://www.thecoderscorner.com/ref-docs/ioabstraction/html/index.html
 */

#include <IoAbstraction.h>
#include <AnalogDeviceAbstraction.h>
#include <TaskManagerIO.h>
#include <DeviceEvents.h>
#include <Wire.h>

// This is the output pin, where analog output will be sent.
// on SAMD MKR boards this is the DAC, for Uno, MEGA change to a PWM pin
#define PWM_OR_DAC_PIN 15

// This is the input pin where analog input is received.
#define ANALOG_IN_PIN A0

AnalogDevice& analog = internalAnalogDevice();


// Here we create an analog event that will be triggered when the the analog level exceeds 75%. it is triggered every
// 100 milliseconds and whwn triggered runs the code in the exec() method.
class MyAnalogExceedsEvent : public AnalogInEvent {
public:
    MyAnalogExceedsEvent(AnalogDevice& device, pinid_t pin) :
            AnalogInEvent(device, pin, 0.75, AnalogInEvent::ANALOGIN_EXCEEDS, 100000UL) {
    }

    void exec() override {
        serdebugF3("Trigger AnalogInEvent Threshold, Last:  ", analogThreshold, lastReading);
    }
};

// We keep a variable that counts the output waveform
float ledCycleValue = 0;
// the current direction of adjustment
float ledCycleAdj = 0.01;



void setup() {
    Serial.begin(115200);

    // set up the device pin directions upfront.
    internalAnalogDevice().initPin(ANALOG_IN_PIN, DIR_IN);
    internalAnalogDevice().initPin(PWM_OR_DAC_PIN, DIR_OUT);

    // this is how to register an event with task manager
    taskManager.registerEvent(new MyAnalogExceedsEvent(internalAnalogDevice(), ANALOG_IN_PIN), true);

    // we schedule a task to run every 500 millis that reads the value from A1 and prints it output
    // along with the largest possible value
    taskManager.scheduleFixedRate(500, [] {
        serdebugF4("Analog input value is ", internalAnalogDevice().getCurrentValue(ANALOG_IN_PIN),
                   internalAnalogDevice().getMaximumRange(DIR_IN, ANALOG_IN_PIN),
                   internalAnalogDevice().getCurrentFloat(ANALOG_IN_PIN) * 100.0F);

#ifdef ESP32
        // On ESP32 boards, where the analogWrite function doesn't exist we use the underlying functions
        // to access either the DAC or LEDC subsystem, if you want to get hold of the ledc channel you can.
        EspAnalogOutputMode* outputMode = internalAnalogDevice().getEspOutputMode(PWM_OR_DAC_PIN);
        if(outputMode != nullptr) {
            serdebugF2("ESP32 Output type: ", outputMode->isDac());
            serdebugF2("LEDC (pwm channel): ", outputMode->getPwmChannel());
        }

        EspAnalogInputMode* inputMode = internalAnalogDevice().getEspInputMode(ANALOG_IN_PIN);
        if(inputMode != nullptr) {
            serdebugF3("ESP32 Input (ondac1, channel): ", inputMode->isOnDAC1(), inputMode->getChannel());
        }
#endif
    });

    // we also create a sawtooth waveform on one of the outputs. By default we are using the DAC
    // on A0 of most MKR boards. Change to PWM for AVR boards.
    taskManager.scheduleFixedRate(10, [] {
        ledCycleValue += ledCycleAdj;
        if(ledCycleValue >= 0.98) ledCycleAdj = -0.01;
        if(ledCycleValue <= 0.02) ledCycleAdj = 0.01;

        internalAnalogDevice().setCurrentFloat(PWM_OR_DAC_PIN, ledCycleValue);
    }, TIME_MILLIS);
}

// and lastly the standard loop for use with task manager, basically does nothing but
// repeatedly call runLoop.
void loop() {
    taskManager.runLoop();
}