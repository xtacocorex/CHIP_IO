#!/usr/bin/python

import CHIP_IO.OverlayManager as OM
import os

# ENABLE DEBUG
print("ENABLING OVERLAY MANAGER DEBUG")
OM.enable_debug()

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

# **************** I2C-1 *******************
print("\nIS I2C ENABLED: {0}".format(OM.get_i2c_loaded()))
OM.load("I2C1")
print("IS I2C ENABLED: {0}".format(OM.get_i2c_loaded()))
# VERIFY I2C-1 EXISTS
if os.path.exists('/sys/class/i2c-dev/i2c-1'):
    print("I2C1 DEVICE EXISTS")
else:
    print("I2C1 DEVICE DID NOT LOAD PROPERLY")
print("UNLOADING I2C1")
OM.unload("I2C1")
print("IS I2C ENABLED: {0}".format(OM.get_i2c_loaded()))

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


