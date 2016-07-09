import pytest

import CHIP_IO.GPIO as GPIO


def teardown_module(module):
    GPIO.cleanup()


class TestGPIOOutput:
    def test_output_high(self):
        GPIO.setup("CSID6", GPIO.OUT)
        GPIO.output("CSID6", GPIO.HIGH)
        value = open('/sys/class/gpio/gpio138/value').read()
        assert int(value)
        GPIO.cleanup()

    def test_output_low(self):
        GPIO.setup("CSID6", GPIO.OUT)
        GPIO.output("CSID6", GPIO.LOW)
        value = open('/sys/class/gpio/gpio138/value').read()
        assert not int(value)
        GPIO.cleanup()

    def test_direction_readback(self):
        GPIO.setup("CSID6", GPIO.OUT)
        direction = GPIO.gpio_function("CSID6")
        assert direction == GPIO.OUT
        GPIO.cleanup()

    def test_output_greater_than_one(self):
        GPIO.setup("CSID6", GPIO.OUT)
        GPIO.output("CSID6", 2)
        value = open('/sys/class/gpio/gpio138/value').read()
        assert int(value)
        GPIO.cleanup()

    def test_output_of_pin_not_setup(self):
        with pytest.raises(RuntimeError):
            GPIO.output("CSID7", GPIO.LOW)
            GPIO.cleanup()

    def test_output_setup_as_input(self):
        GPIO.setup("CSID6", GPIO.IN)
        with pytest.raises(RuntimeError):
            GPIO.output("CSID6", GPIO.LOW)
            GPIO.cleanup()
