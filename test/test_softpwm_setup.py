import pytest
import os

import CHIP_IO.SOFTPWM as PWM
import CHIP_IO.GPIO as GPIO


def teardown_module(module):
    PWM.cleanup()


class TestSoftpwmSetup:
    def test_start_pwm(self):
        PWM.start("XIO-P7", 50, 10)
        base = GPIO.get_gpio_base() + 7
        gfile = '/sys/class/gpio/gpio%d' % base
        assert os.path.exists(gfile)
        direction = open(gfile + '/direction').read()
        assert(direction == 'out\n')
        PWM.cleanup()

    def test_pwm_start_invalid_pwm_key(self):
        with pytest.raises(ValueError):
            PWM.start("P8_25", -1)

    def test_pwm_start_invalid_duty_cycle_negative(self):
        with pytest.raises(ValueError):
            PWM.start("XIO-P7", -1)

    def test_pwm_start_valid_duty_cycle_min(self):
        # testing an exception isn't thrown
        PWM.start("XIO-P7", 0)
        PWM.cleanup()

    def test_pwm_start_valid_duty_cycle_max(self):
        # testing an exception isn't thrown
        PWM.start("XIO-P7", 100)
        PWM.cleanup()

    def test_pwm_start_invalid_duty_cycle_high(self):
        with pytest.raises(ValueError):
            PWM.start("XIO-P7", 101)

    def test_pwm_start_invalid_duty_cycle_string(self):
        with pytest.raises(TypeError):
            PWM.start("XIO-P7", "1")

    def test_pwm_start_invalid_frequency_negative(self):
        with pytest.raises(ValueError):
            PWM.start("XIO-P7", 0, -1)

    def test_pwm_start_invalid_frequency_string(self):
        with pytest.raises(TypeError):
            PWM.start("XIO-P7", 0, "1")

    def test_pwm_start_negative_polarity(self):
        with pytest.raises(ValueError):
            PWM.start("XIO-P7", 0, 100, -1)

    def test_pwm_start_invalid_positive_polarity(self):
        with pytest.raises(ValueError):
            PWM.start("XIO-P7", 0, 100, 2)

    def test_pwm_start_invalid_polarity_type(self):
        with pytest.raises(TypeError):
            PWM.start("XIO-P7", 0, 100, "1")

    def test_pwm_duty_cycle_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_duty_cycle("XIO-P7", 100)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_key(self):
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_15", 100)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_high(self):
        PWM.start("XIO-P7", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("XIO-P7", 101)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_negative(self):
        PWM.start("XIO-P7", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("XIO-P7", -1)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_string(self):
        PWM.start("XIO-P7", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("XIO-P7", "a")
            PWM.cleanup()

    def test_pwm_frequency_invalid_value_negative(self):
        PWM.start("XIO-P7", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("XIO-P7", -1)
            PWM.cleanup()

    def test_pwm_frequency_invalid_value_string(self):
        PWM.start("XIO-P7", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("XIO-P7", "11")
            PWM.cleanup()

    def test_stop_pwm(self):
        pass
