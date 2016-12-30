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

import os
import time

# Global Variables
DEBUG = False
DEVICE_EXIST = True

# Default Sample Rate Variables
SAMPLE_RATE_32P25 = 32.25
SAMPLE_RATE_62O5  = 62.5
SAMPLE_RATE_125   = 125
SAMPLE_RATE_250   = 250
SAMPLE_RATES      = []

# Scale Factor
SCALE_FACTOR = 31.25

# File Locations
LRADC_BASE_DEVICE_FILE = "/sys/bus/iio/devices/iio:device0"
AVAILABLE_SAMPLE_RATE_FILE = "/sampling_frequency_available"
SCALE_FACTOR_FILE = "/in_voltage_scale"
CURRENT_SAMPLE_RATE_FILE = "/in_voltage_sampling_frequency"
RAW_VOLTAGE_CHAN0_FILE = "/in_voltage0_raw"
RAW_VOLTAGE_CHAN1_FILE = "/in_voltage1_raw"

def enable_debug():
    global DEBUG
    DEBUG = True

def setup(rate=250):
    # First we determine if the device exists
    if not os.path.exists(LRADC_BASE_DEVICE_FILE):
        global DEVICE_EXIST
        DEVICE_EXIST = False
        raise Exception("LRADC Device does not exist")
    else:
        # Set the Sample Rate
        set_sample_rate(rate)

def get_device_exist():
    return DEVICE_EXIST

def get_scale_factor():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the data from the file
    f = open(LRADC_BASE_DEVICE_FILE+SCALE_FACTOR_FILE,"r")
    dat = f.readline()
    f.close()

    # Set the Scale Factor
    global SCALE_FACTOR
    SCALE_FACTOR = float(dat.strip())

    # Debug
    if DEBUG:
        print("Current LRADC Scaling Factor: {0}").format(SCALE_FACTOR)

    return SCALE_FACTOR

def get_allowable_sample_rates():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the data from the file
    f = open(LRADC_BASE_DEVICE_FILE+AVAILABLE_SAMPLE_RATE_FILE,"r")
    dat = f.readline()
    f.close()

    global SAMPLE_RATES
    tmp = dat.strip().split(" ")
    for i in xrange(len(tmp)):
        if "." in tmp[i]:
            tmp[i] = float(tmp[i])
        else:
            tmp[i] = int(tmp[i])
    SAMPLE_RATES = tmp

    # Debug
    if DEBUG:
        print("Allowable Sampling Rates:")
        for rate in SAMPLE_RATES:
            print("{0}").format(rate)

    return tuple(SAMPLE_RATES)

def set_sample_rate(rate):
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Debug
    if DEBUG:
        print("Setting Sample Rate to: {0}").format(rate)

    # Check to see if the rates were gathered already
    global SAMPLE_RATES
    if SAMPLE_RATES == []:
        tmp = get_allowable_sample_rates()

    # Range check the input rate
    if rate not in SAMPLE_RATES:
        raise ValueError("Input Rate an Acceptable Value")

    # Write the rate
    f = open(LRADC_BASE_DEVICE_FILE+CURRENT_SAMPLE_RATE_FILE,"w")
    mystr = "%.2f" % rate
    f.write(mystr)
    f.close()

    # Verify write went well
    crate = get_sample_rate()
    if crate != rate:
        raise Exception("Unable to write new Sampling Rate")

def get_sample_rate():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the data from the file
    f = open(LRADC_BASE_DEVICE_FILE+CURRENT_SAMPLE_RATE_FILE,"r")
    dat = f.read()
    f.close()

    dat = dat.strip()
    if "." in dat:
        dat = float(dat)
    else:
        dat = int(dat)

    # Debug
    if DEBUG:
        print("Current Sampling Rate: {0}").format(dat)

    return dat

def get_chan0_raw():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the data from the file
    f = open(LRADC_BASE_DEVICE_FILE+RAW_VOLTAGE_CHAN0_FILE,"r")
    dat = f.readline()
    f.close()

    dat = float(dat.strip())

    # Debug
    if DEBUG:
        print("CHAN0 RAW: {0}").format(dat)

    return dat

def get_chan1_raw():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the data from the file
    f = open(LRADC_BASE_DEVICE_FILE+RAW_VOLTAGE_CHAN1_FILE,"r")
    dat = f.readline()
    f.close()

    dat = float(dat.strip())

    # Debug
    if DEBUG:
        print("CHAN1 RAW: {0}").format(dat)

    return dat

def get_chan0():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the raw data first
    dat = get_chan0_raw()
    # Apply scale factor
    dat *= SCALE_FACTOR

    # Debug
    if DEBUG:
        print("CHAN0: {0}").format(dat)

    return dat

def get_chan1():
    # If we do not have a device, lets throw an exception
    if not DEVICE_EXIST:
        raise Exception("LRADC Device does not exist")

    # Get the raw data first
    dat = get_chan1_raw()
    # Apply scale factor
    dat *= SCALE_FACTOR

    # Debug
    if DEBUG:
        print("CHAN1: {0}").format(dat)

    return dat


