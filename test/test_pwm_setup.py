import pytest
import os

import CHIP_IO.PWM as PWM

def teardown_module(module):
    PWM.cleanup()

class TestPwmSetup:
    def test_start_pwm(self):
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000
        PWM.cleanup()

    def test_start_pwm_with_polarity_one(self):
        PWM.start("PWM0", 0, 2000, 1)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert string(polarity) == "inverted"
        PWM.cleanup()

    def test_start_pwm_with_polarity_default(self):
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert string(polarity) == "normal"
        PWM.cleanup()

    def test_start_pwm_with_polarity_zero(self):
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert string(polarity) == "normal"
        PWM.cleanup()

    def test_pwm_start_invalid_pwm_key(self):
        with pytest.raises(ValueError):
            PWM.start("P8_25", -1)

    def test_pwm_start_invalid_duty_cycle_negative(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", -1)

    def test_pwm_start_valid_duty_cycle_min(self):
        #testing an exception isn't thrown
        PWM.start("PWM0", 0)
        PWM.cleanup()

    def test_pwm_start_valid_duty_cycle_max(self):
        #testing an exception isn't thrown
        PWM.start("PWM0", 100)
        PWM.cleanup()

    def test_pwm_start_invalid_duty_cycle_high(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", 101)

    def test_pwm_start_invalid_duty_cycle_string(self):
        with pytest.raises(TypeError):
            PWM.start("PWM0", "1")

    def test_pwm_start_invalid_frequency_negative(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", 0, -1)

    def test_pwm_start_invalid_frequency_string(self):
        with pytest.raises(TypeError):
            PWM.start("PWM0", 0, "1")

    def test_pwm_start_negative_polarity(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", 0, 100, -1)

    def test_pwm_start_invalid_positive_polarity(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", 0, 100, 2)

    def test_pwm_start_invalid_polarity_type(self):
        with pytest.raises(TypeError):
            PWM.start("PWM0", 0, 100, "1")

    def test_pwm_duty_modified(self):
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000

        PWM.set_duty_cycle("PWM0", 100)
        duty = open(pwm_test + '/duty_cycle').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 500000
        assert int(period) == 500000
        PWM.cleanup()

    def test_pwm_duty_cycle_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_duty_cycle("PWM0", 100)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_key(self):
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_15", 100)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_high(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", 101)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_negative(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", -1)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_string(self):
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("PWM0", "a")
            PWM.cleanup()

    def test_pwm_frequency_invalid_value_negative(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("PWM0", -1)
            PWM.cleanup()

    def test_pwm_frequency_invalid_value_string(self):
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("PWM0", "11")
            PWM.cleanup()

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_frequency("PWM0", 100)
            PWM.cleanup()

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_15", 100)
            PWM.cleanup()

    def test_stop_pwm(self):
        pass
        #PWM.start("PWM0", 1)
        #PWM.stop("PWM0")
        #assert os.path.exists('/sys/class/gpio/gpio68')
        #direction = open('/sys/class/gpio/gpio68/direction').read()
        #assert direction == 'out\n'
        #PWM.cleanup()
