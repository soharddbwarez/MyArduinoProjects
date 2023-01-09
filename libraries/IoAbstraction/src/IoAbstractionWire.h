/*
 * Copyright (c) 2018 https://www.thecoderscorner.com (Dave Cherry).
 * This product is licensed under an Apache license, see the LICENSE file in the top-level directory.
 */

/**
 * @file IoAbstractionWire.h
 * 
 * @brief Contains the versions of BasicIoAbstraction that use i2c communication. Including PCF8574 and MCP23017.
 */

#ifndef _IOABSTRACTION_IOABSTRACTIONWIRE_H_
#define _IOABSTRACTION_IOABSTRACTIONWIRE_H_

#define AW9523_LED_OUTPUT 0x99

#include "PlatformDeterminationWire.h"
#include "IoAbstraction.h"
#include "AnalogDeviceAbstraction.h"

/**
 * An implementation of BasicIoAbstraction that supports the PCF8574/PCF8575 i2c IO chip. Providing all possible capabilities
 * of the chip in a similar manner to Arduino pins. 
 * @see ioFrom8574 for how to create an instance
 * @see ioDevicePinMode for setting pin modes
 */
class PCF8574IoAbstraction : public BasicIoAbstraction {
public:
    enum { NEEDS_WRITE_FLAG, PINS_CONFIGURED_READ_FLAG, PCF8575_16BIT_FLAG, INVERTED_LOGIC };
private:
	WireType wireImpl;
	uint8_t address;
	uint8_t lastRead[2];
	uint8_t toWrite[2];
	uint8_t flags;
	uint8_t interruptPin;
public:
	/** 
	 * Construct a 8574 expander on i2c address and with interrupts connected to a given pin (0xff no interrupts) 
	 * @param addr the I2C address on the bus
	 * @param interruptPin the pin on the Arduino that the interrupt line is connected to
	 * @param wireInstance the instance of wire to use for this device, for example &Wire.
	 * @param mode16bit transfer 16 bits of data. Used in 8575 expander.
	 * @param invertedLogic invert bits sent and received from the expander.
	 */
	PCF8574IoAbstraction(uint8_t addr, uint8_t interruptPin, WireType wireInstance = nullptr, bool mode16bit = false, bool invertedLogic = false);
	virtual ~PCF8574IoAbstraction() { }

	/** Forces the device to start reading back state during syncs even if no pins are configured as read */
	void overrideReadFlag() { bitWrite(flags, PINS_CONFIGURED_READ_FLAG, true); }

	/** 
	 * sets the pin direction on the device, notice that on this device input is achieved by setting the port to high 
	 * so it is always set as INPUT_PULLUP, even if INPUT is chosen 
	 */
	void pinDirection(pinid_t pin, uint8_t mode) override;

	/** 
	 * writes a new value to the device after a sync. 
	 */
	void writeValue(pinid_t pin, uint8_t value) override;

	/**
	 * reads a value from the last cached state  - updated each sync
	 */
	uint8_t readValue(pinid_t pin) override;

	/**
	 * Writes a complete 8 bit port value, that is updated to the device each sync
	 */
	void writePort(pinid_t pin, uint8_t port) override;

	/**
	 * Reads the complete 8 bit byte from the last cached state, that is updated each sync.
	 */ 
	uint8_t readPort(pinid_t pin) override;

	/** 
	 * attaches an interrupt handler for this device. Notice for this device, all pin changes will be notified
	 * on any pin of the port, it is not configurable at the device level, the type of interrupt will also
	 * always be CHANGE.
	 */
	void attachInterrupt(pinid_t pin, RawIntHandler intHandler, uint8_t mode) override;

	/** 
	 * updates settings on the board after changes 
	 */
	bool runLoop() override;
};

class Standard16BitDevice : public BasicIoAbstraction {
protected:
    uint16_t lastRead;
    uint16_t toWrite;
    uint8_t  flags;
    virtual void initDevice()=0;
public:
    Standard16BitDevice();
    ~Standard16BitDevice() = default;

    void writeValue(pinid_t pin, uint8_t value) override;
    uint8_t readValue(pinid_t pin) override;
    void writePort(pinid_t pin, uint8_t port) override;
    uint8_t readPort(pinid_t pin) override;
    void clearChangeFlags();
    void setReadPort(int port);
    bool isReadPortSet(int port) const;
    bool isWritePortSet(int port) const;
    bool isInitNeeded() const;
    void markInitialised();
};

/**
 * The interrupt mode in which the 23x17 device is going to operate. See the device datasheet for more information.
 * Using the ACTIVE_LOW_OPEN the library will ensure INPUT_PULLUP is used on the Arduino side.
 */
enum Mcp23xInterruptMode {
	NOT_ENABLED = 0, ACTIVE_HIGH_OPEN = 0b110, ACTIVE_LOW_OPEN = 0b100, ACTIVE_HIGH = 0b010, ACTIVE_LOW = 0b000 
};

/**
 * This abstaction supports most of the available features on the 23x17 range of IOExpanders. It supports most
 * of the GPIO functions and nearly all of the interrupt modes, and is therefore very close to Arduino pins in
 * terms of functionality.
 */
class MCP23017IoAbstraction : public Standard16BitDevice {
private:
	WireType wireImpl;
	uint8_t  address;
	pinid_t  intPinA;
	pinid_t  intPinB;
	uint8_t  intMode;
public:
	/**
	 * Most complete constructor, allows for either single or dual interrupt mode and all capabilities
	 * @see iofrom23017
	 * @see iofrom23017IntPerPort
	 */
	MCP23017IoAbstraction(uint8_t address, Mcp23xInterruptMode intMode,  pinid_t intPinA, pinid_t intPinB, WireType wireImpl = nullptr);
    /**
     * Simplest constructor: create a MCP23017 device with no interrupt mode enabled, only the I2C address needed.
     * @param address the I2C address
     */
    MCP23017IoAbstraction(uint8_t address, WireType wireImpl = nullptr);

    /**
     * Create a MCP23017 device that will use a single interrupt mode and optionally provide the wire implementation
     * @param address
     * @param intPinA
     */
    MCP23017IoAbstraction(uint8_t address, Mcp23xInterruptMode intMode, pinid_t intPinA, WireType wireImpl = nullptr);


	~MCP23017IoAbstraction() override = default;

	/**
	 * Sets the pin direction similar to pinMode, pin direction on this device supports INPUT_PULLUP, INPUT and OUTPUT.
	 * @param pin the pin to set direction for on this device
	 * @param mode the mode such as INPUT, INPUT_PULLUP, OUTPUT
	 */
	void pinDirection(pinid_t pin, uint8_t mode) override;

	/**
	 * Attaches an interrupt to the device and links it to the arduino pin. On the MCP23017 nearly all interrupt modes
	 * are supported, including CHANGE, RISING, FALLING and are selective both per port and by pin.
	 */
	void attachInterrupt(pinid_t pin, RawIntHandler intHandler, uint8_t mode) override;
	
	/** 
	 * updates settings on the board after changes 
	 */
	bool runLoop() override;
	
    /**
     * This MCP23017 only function inverts the meaning of a given input pin. The pins for this
     * are 0..15 and true will invert the meaning, whereas false will leave as is. regardless if
     * you are using any other IO expanders, using this function requires that you have an actual
     * MCP23017IoAbstraction reference. If you want to use this feature, instead of the variable
     * being of type IoAbstractionRef, it should be of type MCP23017IoAbstraction*
     * 
     * @param pin the input pin between 0..15
     * @param shouldInvert true to invert the given pin, otherwise false.
     */
    void setInvertInputPin(pinid_t pin, bool shouldInvert);

    /**
     * If you've connected the reset pin to a GPIO, then provide the pin to this function and it will
     * reset the device then leave the pin in the right state for the device to function. It will yield
     * to task manager for 100 microseconds. It also ensures that the pin is set as output first.
     * @param resetPin the reset GPIO pin.
     */
    void resetDevice(int resetPin);

private:
	void initDevice() override;
};

/**
 * The AW9523IoAbstraction class provides an Arduino like wrapper for the AW9523 I2C device providing access to most
 * functions available on the device in an Arduino like way, implementing IoAbstraction so that it can be used with
 * LCD units, switches, rotary encoders and matrix keyboards in the regular way. The LED current control facilities are
 * also exposed using an extension.
 */
class AW9523IoAbstraction : public Standard16BitDevice {
private:
    WireType wireImpl;
    uint8_t i2cAddress;
    pinid_t interruptPin;
public:
    enum AW9523CurrentControl: uint8_t { FULL_CURRENT = 0, CURRENT_THREE_QUARTER = 1, CURRENT_HALF = 2, CURRENT_QUARTER = 3 };

    /**
     * create an instance of the AW9523 abstraction that communicates with the device and extends Arduino like functions
     * for the pins on the device. This constructor takes the I2C address, and optionally an interrupt pin and wire
     * implementation. Any interrupt pin provided will be set up on your behalf automatically.
     *
     * @param addr the address on the I2C bus of the device
     * @param intPin optionally a pin on the board that the devices interrupt pin is connected to
     * @param wirePtr optionally an alternative wire implementation, EG Wire1, or other non standard I2C device.
     */
    explicit AW9523IoAbstraction(uint8_t addr, pinid_t intPin = IO_PIN_NOT_DEFINED, WireType wirePtr = nullptr);
    ~AW9523IoAbstraction() override = default;

    /**
     * Gets the device ID from the chip from the ID register.
     * @return the device ID
     */
    uint8_t deviceId();


    /**
     * Sets the pin direction similar to pinMode, pin direction on this device supports INPUT, OUTPUT and
     * AW9523_LED_OUTPUT which enables the onboard LED controller, and then you use the setPinLedCurrent to control
     * instead of digital write.
     * @param pin the pin to set direction for on this device
     * @param mode for this device, INPUT, OUTPUT or AW9523_LED_OUTPUT
     */
    void pinDirection(pinid_t pin, uint8_t mode) override;

    /**
     * Attaches an interrupt to the device and links it to the arduino pin. On the MCP23017 nearly all interrupt modes
     * are supported, including CHANGE, RISING, FALLING and are selective both per port and by pin.
     */
    void attachInterrupt(pinid_t pin, RawIntHandler intHandler, uint8_t mode) override;

    /**
     * updates settings on the board after changes
     */
    bool runLoop() override;

    /**
     * Enable or disable the LED controller mode of the pin, the power parameter is a value between 0 and 255 that
     * represents the dimming level for that pin.
     * @param pin the pin to control
     * @param pwr the current to provide, based on the global control setting, default 0..37mA.
     */
    void setPinLedCurrent(pinid_t pin, uint8_t pwr);

    /**
     * You can change the global control register using this function, setting P0 either as push pull or open drain,
     * you can also change the max-current range setting for the chip. This is as per datasheet. Make sure you've called
     * sync at least once before calling.
     * @param pushPullP0 true - enable push pull, false - open drain
     * @param maxCurrentMode - see the AW9523CurrentControl enumeration for appropriate values
     */
    void writeGlobalControl(bool pushPullP0, AW9523CurrentControl maxCurrentMode = FULL_CURRENT);

    /**
     * Perform a software reset of the device. Make sure you've called sync at least once before calling.
     */
    void softwareReset();
private:
    void initDevice() override;
};

/**
 * A wrapper for the AW9523IoAbstraction that provides the LED control analog functions as an AnalogDevice, this allows
 * you to use this device with anything that already works with AnalogDevice objects for output. Simple construct giving
 * a reference to the actual abstraction.
 */
class AW9523AnalogAbstraction : public AnalogDevice {
private:
    AW9523IoAbstraction& theAbstraction;
public:
    explicit AW9523AnalogAbstraction(AW9523IoAbstraction &abs): theAbstraction(abs) {}

    int getMaximumRange(AnalogDirection direction, pinid_t pin) override { return 255; }

    int getBitDepth(AnalogDirection direction, pinid_t pin) override { return 8; }

    void initPin(pinid_t pin, AnalogDirection direction) override;

    unsigned int getCurrentValue(pinid_t pin) override { return -1; }

    float getCurrentFloat(pinid_t pin) override { return NAN; }

    void setCurrentValue(pinid_t pin, unsigned int newValue) override { theAbstraction.setPinLedCurrent(pin, newValue); }

    void setCurrentFloat(pinid_t pin, float newValue) override { theAbstraction.setPinLedCurrent(pin, (uint8_t)(newValue * 255.0F)); }
};

// to remain compatible with old code
#define ioFrom8754 ioFrom8575

/**
 * Creates an instance of an IoAbstraction that works with a PCF8574 chip over i2c, which optionally
 * has support for interrupts should it be needed. Note that only interrupt mode CHANGE is support, 
 * and a change on any pin raises an interrupt. All inputs are by default INPUT_PULLUP by device design.
 * In new code prefer using the constructor.
 * @param addr the i2c address of the device
 * @param interruptPin (optional default = 0xff) the pin on the Arduino side that is used for interrupt handling if needed.
 * @param wireImpl (optional defaults to Wire) pointer to a TwoWire class to use if not using Wire
 * @param invertedLogic invert bits sent and received from the expander.
 * @return an IoAbstactionRef for the device
 */
IoAbstractionRef ioFrom8574(uint8_t addr, pinid_t interruptPin, WireType wireImpl, bool invertedLogic = false);

/**
 * Creates an instance of an IoAbstraction that works with a PCF8575 16 bit chip over i2c, which optionally
 * has support for interrupts should it be needed. Note that only interrupt mode CHANGE is support,
 * and a change on any pin raises an interrupt. All inputs are by default INPUT_PULLUP by device design.
 * In new code prefer using the constructor.
 * @param addr the i2c address of the device
 * @param interruptPin (optional default = 0xff) the pin on the Arduino side that is used for interrupt handling if needed.
 * @param wireImpl (optional defaults to Wire) pointer to a TwoWire class to use if not using Wire
 * @param invertedLogic invert bits sent and received from the expander.
 * @return an IoAbstactionRef for the device
 */
IoAbstractionRef ioFrom8575(uint8_t addr, pinid_t interruptPin, WireType wireImpl, bool invertedLogic = false);

/**
 * Perform digital read and write functions using 23017 expanders. These expanders are the closest in
 * terms of functionality to regular Arduino pins, supporting most interrupt modes and very similar GPIO
 * capabilities. See the other helper methods if you want interrupts.
 * In new code prefer using the constructor.
 * @param addr the i2c address of the device
 * @param wireImpl (defaults to using Wire) can be overriden to any pointer to another Wire/I2C
 * @return an IoAbstactionRef for the device
 */
IoAbstractionRef ioFrom23017(pinid_t addr, WireType wireImpl);

/**
 * Perform digital read and write functions using 23017 expanders. These expanders are the closest in
 * terms of functionality to regular Arduino pins, supporting most interrupt modes and very similar GPIO
 * capabilities. This uses one Arduino interrupt pin for BOTH ports on the device.
 * In new code prefer using the constructor.
 * @param addr the i2c address of the device
 * @param intMode the interrupt mode the device will operate in
 * @param interruptPin the pin on the Arduino that will be used to detect the interrupt condition.
 * @param wireImpl (defaults to using Wire) can be overriden to any pointer to another Wire/I2C
 * @return an IoAbstactionRef for the device
 */
IoAbstractionRef ioFrom23017(uint8_t addr, Mcp23xInterruptMode intMode, pinid_t interruptPin, WireType wireImpl);

/**
 * Perform digital read and write functions using 23017 expanders. These expanders are the closest include
 * terms of functionality to regular Arduino pins, supporting most interrupt modes and very similar GPIO
 * capabilities. If interrupts are needed, this uses one Arduino pin for EACH port on the device.
 * In new code prefer using the constructor.
 * @param addr the i2c address of the device
 * @param intMode the interrupt mode the device will operate in
 * @param interruptPinA the pin on the Arduino that will be used to detect the PORTA interrupt condition.
 * @param interruptPinB the pin on the Arduino that will be used to detect the PORTB interrupt condition.
 * @param wireImpl (defaults to using Wire) can be overriden to any pointer to another Wire/I2C
 * @return an IoAbstactionRef for the device
 */
IoAbstractionRef ioFrom23017IntPerPort(pinid_t addr, Mcp23xInterruptMode intMode, pinid_t interruptPinA, pinid_t interruptPinB, WireType wireImpl);

inline IoAbstractionRef ioFrom8574(uint8_t addr, pinid_t interruptPin = 0xff, bool invertedLogic = false) {
    return ioFrom8574(addr, interruptPin, defaultWireTypePtr, invertedLogic);
};

inline IoAbstractionRef ioFrom8575(uint8_t addr, pinid_t interruptPin = 0xff, bool invertedLogic = false) {
    return ioFrom8575(addr, interruptPin, defaultWireTypePtr, invertedLogic);
};

inline IoAbstractionRef ioFrom23017IntPerPort(uint8_t addr, Mcp23xInterruptMode intMode, pinid_t interruptPinA, pinid_t interruptPinB) {
    return ioFrom23017IntPerPort(addr, intMode, interruptPinA, interruptPinB, defaultWireTypePtr);
}

inline IoAbstractionRef ioFrom23017(uint8_t addr, Mcp23xInterruptMode intMode, pinid_t interruptPin) {
    return ioFrom23017(addr, intMode, interruptPin, defaultWireTypePtr);
}

inline IoAbstractionRef ioFrom23017(pinid_t addr) {
    return ioFrom23017(addr, defaultWireTypePtr);
}

#endif /* _IOABSTRACTION_IOABSTRACTIONWIRE_H_ */
