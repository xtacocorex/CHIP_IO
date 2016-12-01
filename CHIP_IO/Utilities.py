# Copyright (c) 2016 Robert Wolterman
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# CHIP_IO Utilities
# Random functions to enable fun stuff on the CHIP!

# Credit goes to nonokuono (https://bbs.nextthing.co/users/nonokunono)
# for gathering the i2cset commands from the AXP-209 datasheet for 2.0, 2.6, and 3.3V output
# and for figuring out the ADC setup on the AXP-209

import subprocess

# Enable 1.8V Pin on CHIP U13 Header
def enable_1v8_pin():
    # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
    # First we have to write 0x00 to AXP-209 Register 0x91
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0x00', shell=True)
    # Then we have to write 0x03 to AXP-209 Register 0x90
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x03', shell=True)

# Disable 1.8V Pin on CHIP U13 Header
def disable_1v8_pin():
    # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
    # First we have to write 0x05 to AXP-209 Register 0x91
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0x05', shell=True)
    # Then we have to write 0x07 to AXP-209 Register 0x90
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x07', shell=True)

# Change 1.8V Pin on CHIP U13 Header to 2.0V
def change_1v8_pin_to_2v0():
    # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
    # First we have to write 0x20 to AXP-209 Register 0x91
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0x20', shell=True)
    # Then we have to write 0x03 to AXP-209 Register 0x90
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x03', shell=True)

# Change 1.8V Pin on CHIP U13 Header to 2.6V
def change_1v8_pin_to_2v6():
    # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
    # First we have to write 0x80 to AXP-209 Register 0x91
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0x80', shell=True)
    # Then we have to write 0x03 to AXP-209 Register 0x90
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x03', shell=True)

# Change 1.8V Pin on CHIP U13 Header to 3.3V
def change_1v8_pin_to_3v3():
    # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
    # First we have to write 0xF0 to AXP-209 Register 0x91
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0xF0', shell=True)
    # Then we have to write 0x03 to AXP-209 Register 0x90
    subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x03', shell=True)


