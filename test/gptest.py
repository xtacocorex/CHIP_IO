#!/usr/bin/python

import CHIP_IO.GPIO as GPIO
import time, os

print "SETUP CSIDO"
GPIO.setup("CSID0", GPIO.OUT)

#print os.path.exists('/sys/class/gpio/gpio132')

print "SETUP XIO-P1"
GPIO.setup("XIO-P1", GPIO.IN)
#GPIO.setup("U14_13", GPIO.IN)

print "READING XIO-P1"
GPIO.output("CSID0", GPIO.HIGH)
print "HIGH", GPIO.input("XIO-P1")

GPIO.output("CSID0", GPIO.LOW)
GPIO.output("CSID0", GPIO.LOW)
time.sleep(1)
print "LOW", GPIO.input("XIO-P1")

GPIO.output("CSID0", GPIO.HIGH)
GPIO.output("CSID0", GPIO.HIGH)
print "HIGH", GPIO.input("XIO-P1")
time.sleep(1)

GPIO.output("CSID0", GPIO.LOW)
GPIO.output("CSID0", GPIO.LOW)
print "LOW", GPIO.input("XIO-P1")

print "CLEANUP"
GPIO.cleanup()

