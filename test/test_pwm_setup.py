import pytest
import os
import time

import CHIP_IO.PWM as PWM
import CHIP_IO.OverlayManager as OM

def setup_module(module):
    OM.load("PWM0")

def teardown_module(module):
    PWM.cleanup()
    OM.unload("PWM0")

class TestPwmSetup:

    def setup_method(self, test_method):
        time.sleep(0.5)

    #def teardown_method(self, test_method):
    #    PWM.cleanup()
        #OM.unload("PWM0")

    #def setup_module(self, module):
    #    OM.load("PWM0")

    #def teardown_module(self, module):
    #    OM.unload("PWM0")

    def test_start_pwm(self):
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0/'

        assert os.path.exists(pwm_test) == True
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000

    @pytest.mark.xfail(reason="pwm cleanup is doing weirdness for this test")
    def test_start_pwm_with_polarity_one(self):
        PWM.start("PWM0", 0, 2000, 1)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0/'

        #assert os.path.exists(pwm_test) == True
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        polarity = open(pwm_test + 'polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "inverted"

    @pytest.mark.xfail(reason="pwm cleanup is doing weirdness for this test")
    def test_start_pwm_with_polarity_default(self):
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0/'

        #assert os.path.exists(pwm_test) == True
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        polarity = open(pwm_test + 'polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "normal"

    @pytest.mark.xfail(reason="pwm cleanup is doing weirdness for this test")
    def test_start_pwm_with_polarity_zero(self):
        PWM.start("PWM0", 0, 2000, 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0/'

        #assert os.path.exists(pwm_test) == True
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        polarity = open(pwm_test + 'polarity').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000
        assert str(polarity) == "normal"

    def test_pwm_start_invalid_pwm_key(self):
        with pytest.raises(ValueError):
            PWM.start("P8_25", -1)

    def test_pwm_start_invalid_duty_cycle_negative(self):
        with pytest.raises(ValueError):
            PWM.start("PWM0", -1)

    def test_pwm_start_valid_duty_cycle_min(self):
        #testing an exception isn't thrown
        PWM.start("PWM0", 0)

    def test_pwm_start_valid_duty_cycle_max(self):
        #testing an exception isn't thrown
        PWM.start("PWM0", 100)

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

    @pytest.mark.xfail(reason="pwm cleanup is doing weirdness for this test")
    def test_pwm_duty_modified(self):
        PWM.start("PWM0", 0)

        pwm_test = '/sys/class/pwm/pwmchip0/pwm0/'

        assert os.path.exists(pwm_test) == True
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        assert int(duty) == 0
        assert int(period) == 500000

        PWM.set_duty_cycle("PWM0", 100)
        duty = open(pwm_test + 'duty_cycle').readline().strip()
        period = open(pwm_test + 'period').readline().strip()
        assert int(duty) == 500000
        assert int(period) == 500000

    def test_pwm_duty_cycle_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_duty_cycle("PWM0", 100)

    def test_pwm_duty_cycle_invalid_key(self):
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_15", 100)

    def test_pwm_duty_cycle_invalid_value_high(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", 101)

    def test_pwm_duty_cycle_invalid_value_negative(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("PWM0", -1)

    def test_pwm_duty_cycle_invalid_value_string(self):
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("PWM0", "a")

    def test_pwm_frequency_invalid_value_negative(self):
        PWM.start("PWM0", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("PWM0", -1)

    def test_pwm_frequency_invalid_value_string(self):
        PWM.start("PWM0", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("PWM0", "11")

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_frequency("PWM0", 100)

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_15", 100)

