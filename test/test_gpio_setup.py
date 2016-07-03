import pytest
import os

import CHIP_IO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestSetup:
    def test_setup_output_key(self):
        GPIO.setup("U14_37", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'out\n'
        GPIO.cleanup()

    def test_setup_output_name(self):
        GPIO.setup("CSID6", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'out\n'
        GPIO.cleanup()

    def test_setup_input_key(self):
        GPIO.setup("U14_37", GPIO.IN)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'in\n'
        GPIO.cleanup()

    def test_setup_input_name(self):
        GPIO.setup("CSID6", GPIO.IN)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'in\n'
        GPIO.cleanup()

    def test_setup_input_pull_up(self):
        GPIO.setup("U14_37", GPIO.IN, pull_up_down=GPIO.PUD_UP)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'in\n'
        GPIO.cleanup()

    def test_setup_input_pull_down(self):
        GPIO.setup("U14_37", GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        assert os.path.exists('/sys/class/gpio/gpio138')
        direction = open('/sys/class/gpio/gpio138/direction').read()
        assert direction == 'in\n'
        GPIO.cleanup()

    def test_setup_cleanup(self):
        GPIO.setup("U14_37", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio138')
        GPIO.cleanup()
        assert not os.path.exists('/sys/class/gpio/gpio138')

    def test_setup_failed_type_error(self):
        with pytest.raises(TypeError):
            GPIO.setup("U14_37", "WEIRD")
            GPIO.cleanup()

    def test_setup_failed_value_error(self):
        with pytest.raises(ValueError):
            GPIO.setup("U14_37", 3)
            GPIO.cleanup()

    def test_setup_expanded_gpio(self):
        GPIO.setup("XIO-P1", GPIO.OUT)
        base = GPIO.get_gpio_base() + 1
        gfile = '/sys/class/gpio/gpio%d' % base
        assert os.path.exists(gfile)
        GPIO.cleanup()
        assert not os.path.exists(gfile)

