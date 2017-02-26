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
import glob
import re

# Global Variables
DEBUG = False

def toggle_debug():
    global DEBUG
    if DEBUG:
        DEBUG = False
        print("debug disabled")
    else:
        DEBUG = True
        print("debug enabled")

# Set the 1.8V-pin on the CHIP U13-header to given voltage
# Return False on error
def set_1v8_pin_voltage(voltage):
    if not is_chip_pro():
        if not isinstance(voltage, int) and not isinstance(voltage, float):
            return False
        if voltage < 1.8 or voltage > 3.3:
            return False
        if DEBUG:
            print("Setting 1.8V Pin voltage: {0}".format(voltage))
        voltage=int(round((voltage - 1.8) / 0.1)) << 4
        if subprocess.call(["/usr/sbin/i2cset", "-f", "-y" ,"0", "0x34", "0x90", "0x03"]):
            if DEBUG:
                print("Pin enable command failed")
            return False
        if subprocess.call(["/usr/sbin/i2cset", "-f", "-y", "0", "0x34", "0x91", str(voltage)]):
            if DEBUG:
                print("Pin set voltage command failed")
            return False
        return True
    else:
        print("Set 1.8V Pin Voltage not supported on the CHIP Pro")

# Get the voltage the 1.8V-pin on the CHIP U13-header has been configured as
# Return False on error
def get_1v8_pin_voltage():
    if not is_chip_pro():
        p=subprocess.Popen(["/usr/sbin/i2cget", "-f", "-y", "0", "0x34", "0x90"], stdout=subprocess.PIPE)
        output=p.communicate()[0].decode("utf-8").strip()
        #Not configured as an output
        if output != "0x03":
            if DEBUG:
                print("1.8V Pin is currently disabled")
            return False
        p=subprocess.Popen(["/usr/sbin/i2cget", "-f", "-y", "0", "0x34", "0x91"], stdout=subprocess.PIPE)
        output=p.communicate()[0].decode("utf-8").strip()
        voltage=round((int(output, 16) >> 4) * 0.1 + 1.8, 1)
        if DEBUG:
            print("Current 1.8V Pin voltage: {0}".format(voltage))
        return voltage
    else:
        print("Get 1.8V Pin Voltage not supported on the CHIP Pro")

# Enable 1.8V Pin on CHIP U13 Header
def enable_1v8_pin():
    if not is_chip_pro():
        set_1v8_pin_voltage(1.8)
    else:
        print("Enable 1.8V Pin not supported on the CHIP Pro")

# Disable 1.8V Pin on CHIP U13 Header
def disable_1v8_pin():

    if not is_chip_pro():
        if DEBUG:
            print("Disabling the 1.8V Pin")
        # CANNOT USE I2C LIB AS WE NEED TO FORCE THE COMMAND DUE TO THE KERNEL OWNING THE DEVICE
        # First we have to write 0x05 to AXP-209 Register 0x91
        subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x91 0x05', shell=True)
        # Then we have to write 0x07 to AXP-209 Register 0x90
        subprocess.call('/usr/sbin/i2cset -f -y 0 0x34 0x90 0x07', shell=True)
    else:
        print("Disable 1.8V Pin not supported on the CHIP Pro")

# Unexport All
def unexport_all():
    if DEBUG:
        print("Unexporting all the pins")
    gpios = glob.glob("/sys/class/gpio/gpio[0-9]*")
    for g in gpios:
        tmp = g.split("/")
        gpio = tmp[4]
        num = re.sub("[a-z]","",gpio)
        cmd = "echo " + num + " > /sys/class/gpio/unexport"
        subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)

# Determine Processor
def is_chip_pro():
    isgr8 = False
    if DEBUG:
        print("Determining if computer is CHIP or CHIP Pro")
    
    # GET FIRST LINE FROM /proc/meminfo
    f = open("/proc/meminfo","r")
    fline = f.readline()
    f.close()
    
    # FIGURE OUT OUR TOTAL MEMORY SIZE
    parts = fline.split()
    mem = float(parts[1]) / 1024
    
    if mem > 380:
        isgr8 = False
        if DEBUG:
            print("found CHIP!")
    else:
        isgr8 = True
        if DEBUG:
            print("found CHIP Pro!")
        
    # Return isgr8
    return isgr8
