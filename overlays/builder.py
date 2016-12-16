from subprocess import call
import os
import shutil
import glob
import sys

def compile():
    print("Compiling DTS Files")
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-spi2.dtbo", "-b", "o", "-@", "overlays/chip-spi2.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-i2c1.dtbo", "-b", "o", "-@", "overlays/chip-i2c1.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-pwm0.dtbo", "-b", "o", "-@", "overlays/chip-pwm0.dts"])

def copy():
    target_dir = os.environ.get('TARGET_DIR', '')
    overlay_path = target_dir + "/lib/firmware/chip_io"
    print("Checking for DTBO Install Path")
    if not os.path.exists(overlay_path):
        print("Path not found, creating "+overlay_path)
        os.makedirs(overlay_path)
    print("Removing old DTBO files (if applicable)")
    for fl in glob.glob(overlay_path+"/chip-*-.dtbo"):
        os.remove(fl)
    print("Moving DTBO files to "+overlay_path)
    shutil.move("overlays/chip-spi2.dtbo", overlay_path+"/chip-spi2.dtbo")
    shutil.move("overlays/chip-i2c1.dtbo", overlay_path+"/chip-i2c1.dtbo")
    shutil.move("overlays/chip-pwm0.dtbo", overlay_path+"/chip-pwm0.dtbo")
