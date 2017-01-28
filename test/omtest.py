#!/usr/bin/python

import CHIP_IO.OverlayManager as OM
import os

# ENABLE DEBUG
print("ENABLING OVERLAY MANAGER DEBUG")
OM.toggle_debug()

# **************** PWM *******************
print("\nIS PWM ENABLED: {0}".format(OM.get_pwm_loaded()))
OM.load("PWM0")
print("IS PWM ENABLED: {0}".format(OM.get_pwm_loaded()))
# VERIFY PWM0 EXISTS
if os.path.exists('/sys/class/pwm/pwmchip0'):
    print("PWM DEVICE EXISTS")
else:
    print("PWM DEVICE DID NOT LOAD PROPERLY")
print("UNLOADING PWM0")
OM.unload("PWM0")
print("IS PWM ENABLED: {0}".format(OM.get_pwm_loaded()))

# **************** SPI2 *******************
print("\nIS SPI ENABLED: {0}".format(OM.get_spi_loaded()))
OM.load("SPI2")
print("IS SPI ENABLED: {0}".format(OM.get_spi_loaded()))
# VERIFY SPI2 EXISTS
if os.listdir('/sys/class/spi_master') != "":
    print("SPI DEVICE EXISTS")
else:
    print("SPI DEVICE DID NOT LOAD PROPERLY")
print("UNLOADING SPI")
OM.unload("SPI2")
print("IS SPI ENABLED: {0}".format(OM.get_spi_loaded()))


