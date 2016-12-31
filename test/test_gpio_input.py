import pytest
import os
import time

import CHIP_IO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestGPIOInput:
    def test_input(self):
        GPIO.setup("CSID6", GPIO.IN)
        #returned as an int type
        input_value = GPIO.input("CSID6")
        #value read from the file will have a \n new line
        value = open('/sys/class/gpio/gpio138/value').read()
        assert int(value) == input_value
        # time.sleep(30) - what is this for?
        GPIO.cleanup()

    def test_direction_readback(self):
        GPIO.setup("CSID6", GPIO.IN)
        direction = GPIO.gpio_function("CSID6")
        assert direction == GPIO.IN
