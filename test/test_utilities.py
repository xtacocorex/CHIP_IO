import pytest

import CHIP_IO.Utilities as UT

class TestUtilities:
    def test_invalid_set_1v8_with_string(self):
        assert not UT.set_1v8_pin_voltage("yaystring")

    def test_invalid_set_1v8_with_outofbounds_value(self):
        assert not UT.set_1v8_pin_voltage(0.5)
        assert not UT.set_1v8_pin_voltage(4.5)
