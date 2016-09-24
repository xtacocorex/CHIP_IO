import pytest
import os

import CHIP_IO.PWM as PWM
import CHIP_IO.OverlayManager as OM

def teardown_module(module):
    PWM.cleanup()
    OM.unload("PWM0")

class TestPwmSetup:
    def test_start_pwm(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        PWM.cleanup()
        OM.unload("PWM0")

    def test_start_pwm_with_polarity_one(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0, 2000, 1)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        polarity = open(pwm_test + '/polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "inverted"
        PWM.cleanup()
        OM.unload("PWM0")

    def test_start_pwm_with_polarity_default(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        polarity = open(pwm_test + '/polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "normal"
        PWM.cleanup()
        OM.unload("PWM0")

    def test_start_pwm_with_polarity_zero(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        polarity = open(pwm_test + '/polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "normal"
        PWM.cleanup()
        OM.unload("PWM0")

    def test_pwm_start_invalid_pwm_key(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("P8_25", -1)
            OM.unload("PWM0")

    def test_pwm_start_invalid_duty_cycle_negative(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("PWM0", -1)
            OM.unload("PWM0")

    def test_pwm_start_valid_duty_cycle_min(self):
        #testing an exception isn't thrown
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        PWM.cleanup()
        OM.unload("PWM0")

    def test_pwm_start_valid_duty_cycle_max(self):
        #testing an exception isn't thrown
        OM.load("PWM0")
        PWM.start("PWM0", 100)
        PWM.cleanup()
        OM.unload("PWM0")

    def test_pwm_start_invalid_duty_cycle_high(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("PWM0", 101)
            OM.unload("PWM0")

    def test_pwm_start_invalid_duty_cycle_string(self):
        with pytest.raises(TypeError):
            OM.load("PWM0")
            PWM.start("PWM0", "1")
            OM.unload("PWM0")

    def test_pwm_start_invalid_frequency_negative(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("PWM0", 0, -1)
            OM.unload("PWM0")

    def test_pwm_start_invalid_frequency_string(self):
        with pytest.raises(TypeError):
            OM.load("PWM0")
            PWM.start("PWM0", 0, "1")
            OM.unload("PWM0")

    def test_pwm_start_negative_polarity(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("PWM0", 0, 100, -1)
            OM.unload("PWM0")

    def test_pwm_start_invalid_positive_polarity(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.start("PWM0", 0, 100, 2)
            OM.unload("PWM0")

    def test_pwm_start_invalid_polarity_type(self):
        with pytest.raises(TypeError):
            OM.load("PWM0")
            PWM.start("PWM0", 0, 100, "1")
            OM.unload("PWM0")

    def test_pwm_duty_modified(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0'

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000

        PWM.set_duty_cycle("PWM0", 100)
        duty = open(pwm_test + '/duty_cycle').readline().strip()
        period = open(pwm_test + '/period').readline().strip()
        assert int(duty) == 500000
        assert int(period) == 500000
        PWM.cleanup()
        OM.unload("PWM0")

    def test_pwm_duty_cycle_non_setup_key(self):
        with pytest.raises(RuntimeError):
            OM.load("PWM0")
            PWM.set_duty_cycle("PWM0", 100)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_duty_cycle_invalid_key(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.set_duty_cycle("P9_15", 100)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_duty_cycle_invalid_value_high(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", 101)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_duty_cycle_invalid_value_negative(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", -1)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_duty_cycle_invalid_value_string(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("PWM0", "a")
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_frequency_invalid_value_negative(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("PWM0", -1)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_frequency_invalid_value_string(self):
        OM.load("PWM0")
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("PWM0", "11")
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(RuntimeError):
            OM.load("PWM0")
            PWM.set_frequency("PWM0", 100)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(ValueError):
            OM.load("PWM0")
            PWM.set_frequency("P9_15", 100)
            PWM.cleanup()
            OM.unload("PWM0")

    def test_stop_pwm(self):
        pass
        #PWM.start("PWM0", 1)
        #PWM.stop("PWM0")
        #assert os.path.exists('/sys/class/gpio/gpio68')
        #direction = open('/sys/class/gpio/gpio68/direction').readline().strip()
        #assert direction == 'out\n'
        #PWM.cleanup()
