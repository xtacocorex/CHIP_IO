from subprocess import call
import os
import shutil
import glob

def compile():
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-spi2.dtbo", "-b", "o", "-@", "overlays/chip-spi2.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-i2c1.dtbo", "-b", "o", "-@", "overlays/chip-i2c1.dts"])
    call(["dtc", "-O", "dtb", "-o", "overlays/chip-pwm0.dtbo", "-b", "o", "-@", "overlays/chip-pwm0.dts"])

def copy():
    if not os.path.exists("/lib/firmware/chip_io/"):
        os.makedirs("/lib/firmware/chip_io/")
    for fl in glob.glob("/lib/firmware/chip_io/chip-*-.dtbo"):
        os.remove(fl)
    shutil.move("overlays/chip-spi2.dtbo", "/lib/firmware/chip_io/chip-spi2.dtbo")
    shutil.move("overlays/chip-i2c1.dtbo", "/lib/firmware/chip_io/chip-i2c1.dtbo")
    shutil.move("overlays/chip-pwm0.dtbo", "/lib/firmware/chip_io/chip-pwm0.dtbo")

