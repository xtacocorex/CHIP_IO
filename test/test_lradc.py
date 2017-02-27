import pytest

import CHIP_IO.LRADC as LRADC

class TestLRADC:
    def test_scale_factor(self):
        assert LRADC.get_scale_factor() == 31.25
        
    def test_sample_rate_values(self):
        assert LRADC.get_allowable_sample_rates() == (32.25, 62.5, 125, 250)

    def test_set_sample_rate(self):
        LRADC.set_sample_rate(32.25)
        assert LRADC.get_sample_rate() == 32.25
