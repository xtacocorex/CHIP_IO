# CHIP_IO
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

    GPIO.wait_for_edge(channel, GPIO.RISING)

Detecting events::

    GPIO.add_event_detect("CSID0", GPIO.FALLING) 
    #your amazing code here 
    #detect wherever: 
    if GPIO.event_detected("CSID0"):
      print "event detected!"

**PWM**::

    Not implemented yet

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

