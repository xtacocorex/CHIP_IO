CHIP_IO
============================
A CHIP GPIO library

NOTE: Now requires the custom DTC to install the library

Manual::

For Python2.7::

    sudo apt-get update
    sudo apt-get install git build-essential python-dev python-pip flex bison -y
    git clone https://github.com/atenart/dtc
    cd dtc
    make
    sudo  make install PREFIX=/usr
    cd ..
    git clone git://github.com/xtacocorex/CHIP_IO.git
    cd CHIP_IO
    sudo python setup.py install
    cd ..
    sudo rm -rf CHIP_IO

For Python3::

    sudo apt-get update
    sudo apt-get install git build-essential python3-dev python3-pip flex bison -y
    git clone https://github.com/atenart/dtc
    cd dtc
    make
    sudo  make install PREFIX=/usr
    cd ..
    git clone git://github.com/xtacocorex/CHIP_IO.git
    cd CHIP_IO
    sudo python3 setup.py install
    cd ..
    sudo rm -rf CHIP_IO

**Usage**

Using the library is very similar to the excellent RPi.GPIO library used on the Raspberry Pi. Below are some examples.

All scripts that require GPIO, PWM (HW and/or SW), and Overlay Manager need to be run with super user permissions!

**Allowable Pin Names for the Library**

The following "table" is the allowable pin names that are able to be used by the library.

  +-----------+-------------+--------+
  |   Name    |  Alt Name   |  Key   |
  +-----------+-------------+--------+
  | TWI1-SDA  | KPD-I2C-SDA | U13_9  |
  +-----------+-------------+--------+
  | TWI1-SCK  | KPD-I2C-SCL | U13_11 |
  +-----------+-------------+--------+
  | LCD-D2    | LCD-D2      | U13_17 |
  +-----------+-------------+--------+
  | PWM0      | PWM0        | U13_18 |
  +-----------+-------------+--------+
  | LCD-D4    | LCD-D4      | U13_19 |
  +-----------+-------------+--------+
  | LCD-D3    | LCD-D3      | U13_20 |
  +-----------+-------------+--------+
  | LCD-D6    | LCD-D6      | U13_21 |
  +-----------+-------------+--------+
  | LCD-D5    | LCD-D5      | U13_22 |
  +-----------+-------------+--------+
  | LCD-D10   | LCD-D10     | U13_23 |
  +-----------+-------------+--------+
  | LCD-D7    | LCD-D7      | U13_24 |
  +-----------+-------------+--------+
  | LCD-D12   | LCD-D12     | U13_25 |
  +-----------+-------------+--------+
  | LCD-D11   | LCD-D11     | U13_26 |
  +-----------+-------------+--------+
  | LCD-D14   | LCD-D14     | U13_27 |
  +-----------+-------------+--------+
  | LCD-D13   | LCD-D13     | U13_28 |
  +-----------+-------------+--------+
  | LCD-D18   | LCD-D18     | U13_29 |
  +-----------+-------------+--------+
  | LCD-D15   | LCD-D15     | U13_30 |
  +-----------+-------------+--------+
  | LCD-D20   | LCD-D20     | U13_31 |
  +-----------+-------------+--------+
  | LCD-D19   | LCD-D19     | U13_32 |
  +-----------+-------------+--------+
  | LCD-D22   | LCD-D22     | U13_33 |
  +-----------+-------------+--------+
  | LCD-D21   | LCD-D21     | U13_34 |
  +-----------+-------------+--------+
  | LCD-CLK   | LCD-CLK     | U13_35 |
  +-----------+-------------+--------+
  | LCD-D23   | LCD-D23     | U13_36 |
  +-----------+-------------+--------+
  | LCD-VSYNC | LCD-VSYNC   | U13_37 |
  +-----------+-------------+--------+
  | LCD-HSYNC | LCD-HSYNC   | U13_38 |
  +-----------+-------------+--------+
  | LCD-DE    | LCD-DE      | U13_40 |
  +-----------+-------------+--------+
  | UART1-TX  | UART-TX     | U14_3  |
  +-----------+-------------+--------+
  | UART1-RX  | UART-RX     | U14_5  |
  +-----------+-------------+--------+
  | LRADC     | ADC         | U14_11 |
  +-----------+-------------+--------+
  | XIO-P0    | XIO-P0      | U14_13 |
  +-----------+-------------+--------+
  | XIO-P1    | XIO-P1      | U14_14 |
  +-----------+-------------+--------+
  | XIO-P2    | GPIO1       | U14_15 |
  +-----------+-------------+--------+
  | XIO-P3    | GPIO2       | U14_16 |
  +-----------+-------------+--------+
  | XIO-P4    | GPIO3       | U14_17 |
  +-----------+-------------+--------+
  | XIO-P5    | GPIO4       | U14_18 |
  +-----------+-------------+--------+
  | XIO-P6    | GPIO5       | U14_19 |
  +-----------+-------------+--------+
  | XIO-P7    | GPIO6       | U14_20 |
  +-----------+-------------+--------+
  | AP-EINT1  | KPD-INT     | U14_23 |
  +-----------+-------------+--------+
  | AP-EINT3  | AP-INT3     | U14_24 |
  +-----------+-------------+--------+
  | TWI2-SDA  | I2C-SDA     | U14_25 |
  +-----------+-------------+--------+
  | TWI2-SCK  | I2C-SCL     | U14_26 |
  +-----------+-------------+--------+
  | CSIPCK    | SPI-SEL     | U14_27 |
  +-----------+-------------+--------+
  | CSICK     | SPI-CLK     | U14_28 |
  +-----------+-------------+--------+
  | CSIHSYNC  | SPI-MOSI    | U14_29 |
  +-----------+-------------+--------+
  | CSIVSYNC  | SPI-MISO    | U14_30 |
  +-----------+-------------+--------+
  | CSID0     | CSID0       | U14_31 |
  +-----------+-------------+--------+
  | CSID1     | CSID1       | U14_32 |
  +-----------+-------------+--------+
  | CSID2     | CSID2       | U14_33 |
  +-----------+-------------+--------+
  | CSID3     | CSID3       | U14_34 |
  +-----------+-------------+--------+
  | CSID4     | CSID4       | U14_35 |
  +-----------+-------------+--------+
  | CSID5     | CSID5       | U14_36 |
  +-----------+-------------+--------+
  | CSID6     | CSID6       | U14_37 |
  +-----------+-------------+--------+
  | CSID7     | CSID7       | U14_38 |
  +-----------+-------------+--------+

**GPIO Setup**

Import the library, and setup as GPIO.OUT or GPIO.IN::

    import CHIP_IO.GPIO as GPIO
    GPIO.setup("CSID0", GPIO.OUT)

You can also refer to the pin number::

    GPIO.setup("U14_31", GPIO.OUT)

You can also refer to the bin based upon its alternate name::

    GPIO.setup("GPIO1", GPIO.IN)

**GPIO Output**

Setup the pin for output, and write GPIO.HIGH or GPIO.LOW. Or you can use 1 or 0.::

    import CHIP_IO.GPIO as GPIO
    GPIO.setup("CSID0", GPIO.OUT)
    GPIO.output("CSID0", GPIO.HIGH)

**GPIO Input**

Inputs work similarly to outputs.::

    import CHIP_IO.GPIO as GPIO
    GPIO.setup("CSID0", GPIO.IN)

Polling inputs::

    if GPIO.input("CSID0"):
        print("HIGH")
    else:
        print("LOW")

Waiting for an edge (GPIO.RISING, GPIO.FALLING, or GPIO.BOTH::

    GPIO.wait_for_edge(channel, GPIO.RISING)

This only works for the AP-EINT1, AP-EINT3, and XPO Pins on the CHIP

Detecting events::

    GPIO.setup("XIO-P0", GPIO.IN)
    GPIO.add_event_detect("XIO-P0", GPIO.FALLING)
    #your amazing code here
    #detect wherever:
    if GPIO.event_detected("XIO-P0"):
        print "event detected!"

**GPIO Cleanup**

To clean up the GPIO when done, do the following::

    GPIO.cleanup()

**PWM**::

Hardware PWM requires a DTB Overlay loaded on the CHIP to allow the kernel to know there is a PWM device available to use.
::
    import CHIP_IO.PWM as PWM
    #PWM.start(channel, duty, freq=2000, polarity=0)
    #duty values are valid 0 (off) to 100 (on)
    PWM.start("PWM0", 50)
    PWM.set_duty_cycle("PWM0", 25.5)
    PWM.set_frequency("PWM0", 10)
    # To stop PWM
    PWM.stop("PWM0")
    PWM.cleanup()
    #For specific polarity: this example sets polarity to 1 on start:
    PWM.start("PWM0", 50, 2000, 1)

**SOFTPWM**::

    import CHIP_IO.SOFTPWM as SPWM
    #SPWM.start(channel, duty, freq=2000, polarity=0)
    #duty values are valid 0 (off) to 100 (on)
    #you can choose any pin
    SPWM.start("XIO-P7", 50)
    SPWM.set_duty_cycle("XIO-P7", 25.5)
    SPWM.set_frequency("XIO-P7", 10)
    # To Stop SPWM
    SPWM.stop("XIO-P7")
    SPWM.cleanup()
    #For specific polarity: this example sets polarity to 1 on start:
    SPWM.start("XIO-P7", 50, 2000, 1)

Use SOFTPWM at low speeds (hundreds of Hz) for the best results. Do not use for anything that needs high precision or reliability.

If using SOFTPWM and PWM at the same time, import CHIP_IO.SOFTPWM as SPWM or something different than PWM as to not confuse the library.

**LRADC**::

The LRADC was enabled in the 4.4.13-ntc-mlc.  This is a 6 bit ADC that is 2 Volt tolerant.
Sample code below details how to talk to the LRADC.

    import CHIP_IO.LRADC as ADC
    # Enable Debug
    ADC.enable_debug()
    # Check to see if the LRADC Device exists
    # Returns True/False
    ADC.get_device_exists()
    # Setup the LRADC
    # Specify a sampling rate if needed
    ADC.setup(rate)
    # Get the Scale Factor
    factor = ADC.get_scale_factor()
    # Get the allowable Sampling Rates
    sampleratestuple = ADC.get_allowable_sample_rates()
    # Set the sampling rate
    ADC.set_sample_rate(rate)
    # Get the current sampling rate
    currentrate = ADC.get_sample_rate()
    # Get the Raw Channel 0 or 1 data
    raw = ADC.get_chan0_raw()
    raw = ADC.get_chan1_raw()
    # Get the factored ADC Channel data
    fulldata = ADC.get_chan0()
    fulldata = ADC.get_chan1()

**SPI**::

SPI requires a DTB Overlay to access.  CHIP_IO does not contain any SPI specific code as the Python spidev module works when it can see the SPI bus.

**Overlay Manager**::

The Overlay Manager enables you to quickly load simple Device Tree Overlays.  The options for loading are:
PWM0, SPI2, I2C1, CUST

Only one of each type of overlay can be loaded at a time, but all three options can be loaded simultaneously.  So you can have SPI2 and I2C1 without PWM0, but you cannot have SPI2 loaded twice.
::
    import CHIP_IO.OverlayManager as OM
    # The enable_debug() function turns on debug printing
    #OM.enable_debug()
    # To load an overlay, feed in the name to load()
    OM.load("PWM0")
    # To verify the overlay was properly loaded, the get_ functions return booleans
    OM.get_pwm_loaded()
    OM.get_i2c_loaded()
    OM.get_spi_loaded()
    # To unload an overlay, feed in the name to unload()
    OM.unload("PWM0")

To use a custom overlay, you must build and compile it properly per the DIP Docs: http://docs.getchip.com/dip.html#development-by-example
There is no verification that the Custom Overlay is setup properly, it's fire and forget
::
    import CHIP_IO.OverlayManager as OM
    # The full path to the dtbo file needs to be specified
    OM.load("CUST","/home/chip/projects/myfunproject/overlays/mycustomoverlay.dtbo")
    # You can check for loading like above, but it's really just there for sameness
    OM.get_custom_loaded()
    # To unload, just call unload()
    OM.unload("CUST")

**OverlayManager requires a 4.4 kernel with the CONFIG_OF_CONFIGFS option enabled in the kernel config.**

**Utilties**::

CHIP_IO now supports the ability to enable and disable the 1.8V port on U13.  This voltage rail isn't enabled during boot.

To use the utilities, here is sample code::

    import CHIP_IO.Utilities as UT
    # Enable 1.8V Output
    UT.enable_1v8_pin()
    # Set 2.0V Output
    UT.set_1v8_pin_voltage(2.0)
    # Set 2.6V Output
    UT.set_1v8_pin_voltage(2.6)
    # Set 3.3V Output
    UT.set_1v8_pin_voltage(3.3)
    # Disable 1.8V Output
    UT.disable_1v8_pin()
    # Get currently-configured voltage (returns False if the pin is not enabled as output)
    UT.get_1v8_pin_voltage()

**Running tests**

Install py.test to run the tests. You'll also need the python compiler package for py.test.::

    sudo apt-get install python-pytest

Execute the following in the root of the project::

    sudo py.test

**Credits**

The CHIP IO Python library was originally forked from the Adafruit Beaglebone IO Python Library.
The BeagleBone IO Python library was originally forked from the excellent MIT Licensed [RPi.GPIO](https://code.google.com/p/raspberry-gpio-python) library written by Ben Croston.

**License**

CHIP IO port by Robert Wolterman, released under the MIT License.
Beaglebone IO Library Written by Justin Cooper, Adafruit Industries. BeagleBone IO Python library is released under the MIT License.
