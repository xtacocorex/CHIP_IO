#!/usr/bin/python

import CHIP_IO.LRADC as ADC

# == ENABLE DEBUG ==
print("ENABLING LRADC DEBUG OUTPUT")
ADC.toggle_debug()

# == SETUP ==
print("LRADC SETUP WITH SAMPLE RATE OF 125")
ADC.setup(125)

# == SCALE FACTOR ==
print("GETTING SCALE FACTOR")
scalefactor = ADC.get_scale_factor()
print(scalefactor)
print("")

# == ALLOWABLE SAMPLING RATES ==
print("GETTING ALLOWABLE SAMPLE RATES")
rates = ADC.get_allowable_sample_rates()
print(rates)
print("IS 32.25 IN RATE TUPLE")
print(ADC.SAMPLE_RATE_32P25 in rates)
print("")

# == CURRENT SAMPLE RATE ==
print("CURRENT SAMPLING RATE")
crate = ADC.get_sample_rate()
print(crate)
print("")

# == SET SAMPLE RATE ==
print("SETTING SAMPLE RATE TO 62.5")
ADC.set_sample_rate(62.5)
crate = ADC.get_sample_rate()
print(crate)
print("")

# == CHAN 0 RAW ==
print("READING LRADC CHAN0 RAW")
raw0 = ADC.get_chan0_raw()
print(raw0)
print("")

# == CHAN 1 RAW ==
print("READING LRADC CHAN1 RAW")
raw1 = ADC.get_chan1_raw()
print(raw1)
print("")

# == CHAN 0 ==
print("READING LRADC CHAN0 WITH SCALE APPLIED")
full0 = ADC.get_chan0()
print(full0)
print("")

# == CHAN 1 ==
print("READING LRADC CHAN1 WITH SCALE APPLIED")
full1 = ADC.get_chan1()
print(full1)
print("")

# == RESET ==
print("RESETING SAMPLE RATE TO 250")
ADC.set_sample_rate(250)

