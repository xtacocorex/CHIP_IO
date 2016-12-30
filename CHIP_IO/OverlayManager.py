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
import shutil
import time

DEBUG = False

OVERLAYINSTALLPATH = "/lib/firmware/chip_io"
OVERLAYCONFIGPATH  = "/sys/kernel/config/device-tree/overlays"
CUSTOMOVERLAYFILEPATH = ""

PWMSYSFSPATH = "/sys/class/pwm/pwmchip0"
I2C1SYSFSPATH = "/sys/class/i2c-dev/i2c-1"
# USING THE BASE DIRECTORY FOR SPI AS THE DEVICE NUMBER CHANGES ON LOAD/UNLOAD
SPI2SYSFSPATH = "/sys/class/spi_master/"

# LOADED VARIABLES
# DO NOT MODIFY BY HAND WHEN USING
# AS IT COULD BREAK FUNCTIONALITY
_LOADED = {
  "I2C1" : False,
  "SPI2" : False,
  "PWM0" : False,
  "CUST" : False
}

_OVERLAYS = {
  "I2C1" : "chip-i2c1.dtbo",
  "SPI2" : "chip-spi2.dtbo",
  "PWM0" : "chip-pwm0.dtbo",
  "CUST" : ""
}

_FOLDERS = {
  "I2C1" : "chip-i2c",
  "SPI2" : "chip-spi",
  "PWM0" : "chip-pwm",
  "CUST" : "chip-cust"
}

def enable_debug():
    global DEBUG
    DEBUG = True

def get_i2c_loaded():
    """
     get_i2c_loaded - Returns True/False based upon if the i2c-1 Overlay is loaded
    """
    global _LOADED
    return _LOADED["I2C1"]

def get_spi_loaded():
    """
     get_spi_loaded - Returns True/False based upon if the spi2 Overlay is loaded
    """
    global _LOADED
    return _LOADED["SPI2"]

def get_pwm_loaded():
    """
     get_pwm_loaded - Returns True/False based upon if the pwm0 Overlay is loaded
    """
    global _LOADED
    return _LOADED["PWM0"]

def get_custom_loaded():
    """
     get_custom_loaded - Returns True/False based upon if a Custom Overlay is loaded
    """
    global _LOADED
    return _LOADED["CUST"]

def _set_overlay_verify(name, overlay_path, config_path):
    """
     _set_overlay_verify - Function to load the overlay and verify it was setup properly
    """
    global DEBUG
    # VERIFY PATH IS NOT THERE
    if os.path.exists(config_path):
        print("Config path already exists! Not moving forward")
        print("config_path: {0}".format(config_path))
        return -1

    # MAKE THE CONFIGURATION PATH
    os.makedirs(config_path)

    # CAT THE OVERLAY INTO THE CONFIG FILESYSTEM
    with open(config_path + "/dtbo", 'wb') as outfile:
        with open(overlay_path, 'rb') as infile:
            shutil.copyfileobj(infile, outfile)

    # SLEEP TO ENABLE THE KERNEL TO DO ITS JOB
    time.sleep(0.2)

    # VERIFY
    if name == "CUST":
        # BLINDLY ACCEPT THAT IT LOADED
        return 0
    elif name == "PWM0":
        if os.path.exists(PWMSYSFSPATH):
            if DEBUG:
                print("PWM IS LOADED!")
            return 0
        else:
            if DEBUG:
                print("ERROR LOAIDNG PWM0")
            return 1
    elif name == "I2C1":
        if os.path.exists(I2C1SYSFSPATH):
            if DEBUG:
                print("I2C-1 IS LOADED!")
            return 0
        else:
            if DEBUG:
                print("ERROR LOADING I2C-1")
            return 1
    elif name == "SPI2":
        if os.listdir(SPI2SYSFSPATH) != "":
            if DEBUG:
                print("SPI2 IS LOADED!")
            return 0
        else:
            if DEBUG:
                print("ERROR LOADING SPI2")
            return 0

def load(overlay, path=""):
    """
     load - Load a DTB Overlay

     Inputs:
      overlay - Overlay Key: I2C1, SPI2, PWM0, CUST
      path    - Full Path to where the custom overlay is stored

     Returns:
      0 - Successful Load
      1 - Unsuccessful Load
      2 - Overlay was previously set
    """
    global DEBUG
    global _LOADED
    if DEBUG:
        print("LOAD OVERLAY: {0} @ {1}".format(overlay,path))
    # SEE IF OUR OVERLAY NAME IS IN THE KEYS
    if overlay.upper() in _OVERLAYS.keys():
        cpath = OVERLAYCONFIGPATH + "/" + _FOLDERS[overlay.upper()]
        if DEBUG:
            print("VALID OVERLAY")
            print("CONFIG PATH:  {0}".format(cpath))
        # CHECK TO SEE IF WE HAVE A PATH FOR CUSTOM OVERLAY
        if overlay.upper() == "CUST" and path == "":
            raise ValueError("Path must be specified for Custom Overlay Choice")
        elif overlay.upper() == "CUST" and _LOADED[overlay.upper()]:
            print("Custom Overlay already loaded")
            return 2
        elif overlay.upper() == "CUST" and not os.path.exists(path):
            print("Custom Overlay path does not exist")
            return 1

        # SET UP THE OVERLAY PATH FOR OUR USE
        if overlay.upper() != "CUST":
            opath = OVERLAYINSTALLPATH + "/" + _OVERLAYS[overlay.upper()]
        else:
            opath = path
        if DEBUG:
            print("OVERLAY PATH: {0}".format(opath))

        if overlay.upper() == "PWM0" and _LOADED[overlay.upper()]:
            print("PWM0 Overlay already loaded")
            return 2

        if overlay.upper() == "I2C1" and _LOADED[overlay.upper()]:
            print("I2C1 Overlay already loaded")
            return 2

        if overlay.upper() == "SPI2" and _LOADED[overlay.upper()]:
            print("SPI2 Overlay already loaded")
            return 2

        # LOAD THE OVERLAY
        errc = _set_overlay_verify(overlay.upper(), opath, cpath)
        if DEBUG:
            print("_SET_OVERLAY_VERIFY ERRC: {0}".format(errc))
        if errc == 0:
            _LOADED[overlay.upper()] = True

    else:
        raise ValueError("Invalid Overlay name specified! Choose between: I2C1, SPI2, PWM0, CUST")

def unload(overlay):
    global DEBUG
    global _LOADED
    if DEBUG:
        print("UNLOAD OVERLAY: {0}".format(overlay))
    # SEE IF OUR OVERLAY NAME IS IN THE KEYS
    if overlay.upper() in _OVERLAYS.keys():
        # BRUTE FORCE REMOVE AS THE DIRECTORY CONTAINS FILES
        os.system('rmdir \"{}\"'.format(OVERLAYCONFIGPATH + "/" + _FOLDERS[overlay.upper()]))
        _LOADED[overlay.upper()] = False
    else:
        raise ValueError("Invalid Overlay name specified! Choose between: I2C1, SPI2, PWM0, CUST")


