CHIP_IO
============================
A CHIP GPIO library

Manual::

    sudo ntpdate pool.ntp.org
    sudo apt-get update
    sudo apt-get install build-essential python-dev python-pip -y
    git clone git://github.com/xtacocorex/CHIP_IO.git
    cd CHIP_IO
    sudo python setup.py install
    cd ..
    sudo rm -rf CHIP_IO

**Usage**

Using the library is very similar to the excellent RPi.GPIO library used on the Raspberry Pi. Below are some examples.

**GPIO Setup**

Import the library, and setup as GPIO.OUT or GPIO.IN::

    import CHIP_IO.GPIO as GPIO
    GPIO.setup("CSID0", GPIO.OUT)

You can also refer to the pin number::

    GPIO.setup("U14_31", GPIO.OUT)

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

This only works for the AP-EINT1, AP-EINT3, and XPO Pins on the CHIP

    GPIO.wait_for_edge(channel, GPIO.RISING)

Detecting events::

    GPIO.setup("XIO-P0", GPIO.IN)
    GPIO.add_event_detect("XIO-P0", GPIO.FALLING)
    #your amazing code here
    #detect wherever:
    if GPIO.event_detected("XIO-P0"):
      print "event detected!"

**PWM**::

    import CHIP_IO.PWM as PWM
    #PWM.start(channel, duty, freq=2000, polarity=0)
    #duty values are valid 0 (off) to 100 (on)
    PWM.start("PWM0", 50)
    PWM.set_duty_cycle("PWM0", 25.5)
    PWM.set_frequency("PWM0", 10)

    PWM.stop("PWM0")
    PWM.cleanup()

    #set polarity to 1 on start:
    PWM.start("PWM0", 50, 2000, 1)

**SOFTPWM**::

    import CHIP_IO.SOFTPWM as PWM
    #PWM.start(channel, duty, freq=2000, polarity=0)
    #duty values are valid 0 (off) to 100 (on)
    #you can choose any pin
    PWM.start("XIO-P7", 50)
    PWM.set_duty_cycle("XIO-P7", 25.5)
    PWM.set_frequency("XIO-P7", 10)

    PWM.stop("XIO-P7")
    PWM.cleanup()

    #set polarity to 1 on start:
    PWM.start("XIO-P7", 50, 2000, 1)

Use SOFTPWM at low speeds (hundreds of Hz) for the best results. Do not use for anything that needs high precision or reliability.

**ADC**::

    Not Implemented yet

**Running tests**

Install py.test to run the tests. You'll also need the python compiler package for py.test.::

    opkg update && opkg install python-compiler
    #Either pip or easy_install
    pip install -U pytest
    easy_install -U pytest

Execute the following in the root of the project::

    py.test

**Credits**

The CHIP IO Python library was originally forked from the Adafruit Beaglebone IO Python Library.
The BeagleBone IO Python library was originally forked from the excellent MIT Licensed [RPi.GPIO](https://code.google.com/p/raspberry-gpio-python) library written by Ben Croston.

**License**

CHIP IO port by Robert Wolterman, released under the MIT License.
Beaglebone IO Library Written by Justin Cooper, Adafruit Industries. BeagleBone IO Python library is released under the MIT License.
