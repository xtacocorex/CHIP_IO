#!/usr/bin/python

import CHIP_IO.GPIO as GPIO
import time, os, threading

def myfuncallback(channel):
    print "CALLBACK LIKE DRAKE IN HOTLINE BLING"
    
def loopfunction():
    print "LOOP FUNCTION"
    for i in xrange(20):
        if i % 2:
            print "SETTING CSID0 LOW"
            GPIO.output("CSID0",GPIO.LOW)
        else:
            print "SETTING CSID0 HIGH"
            GPIO.output("CSID0",GPIO.HIGH)
        print "SLEEPING"
        time.sleep(1)
    
print "SETUP XIO-P0"
GPIO.setup("XIO-P0", GPIO.IN)

print "SETUP CSID0"
GPIO.setup("CSID0", GPIO.OUT)

# VERIFY SIMPLE FUNCTIONALITY
print "VERIFY SIMPLE FUNCTIONALITY"

print "READING XIO-PI"
GPIO.output("CSID0", GPIO.HIGH)
print "HIGH", GPIO.input("XIO-P0")

GPIO.output("CSID0", GPIO.LOW)
print "LOW", GPIO.input("XIO-P0")

# ==============================================
# EDGE DETECTION - AP-EINT1
print "SETTING UP EDGE DETECTION ON AP-EINT1"
GPIO.setup("AP-EINT1", GPIO.IN)
GPIO.add_event_detect("AP-EINT1",GPIO.FALLING)

print "VERIFYING EDGE DETECT"
f = open("/sys/class/gpio/gpio193/edge","r")
edge = f.read()
f.close()
print "EDGE: %s" % edge
GPIO.remove_event_detect("AP-EINT1")

# ==============================================
# EDGE DETECTION - AP-EINT3
print "SETTING UP EDGE DETECTION ON AP-EINT3"
GPIO.setup("AP-EINT3", GPIO.IN)
GPIO.add_event_detect("AP-EINT3",GPIO.FALLING)

print "VERIFYING EDGE DETECT"
f = open("/sys/class/gpio/gpio35/edge","r")
edge = f.read()
f.close()
print "EDGE: %s" % edge
GPIO.remove_event_detect("AP-EINT3")

# ==============================================
# EDGE DETECTION - EXPANDED GPIO
print "SETTING UP EDGE DETECTION ON XIO-P0"
GPIO.add_event_detect("XIO-P0",GPIO.FALLING,myfuncallback)

print "VERIFYING EDGE DETECT"
f = open("/sys/class/gpio/gpio408/edge","r")
edge = f.read()
f.close()
print "EDGE: %s" % edge

# LOOP WRITING ON CSID0 TO HOPEFULLY GET CALLBACK TO WORK
print "WAITING FOR CALLBACKS"
loopfunction()

print "PRESS CONTROL-C TO EXIT IF SCRIPT GETS STUCK"
GPIO.remove_event_detect("XIO-P0")
try:
    # WAIT FOR EDGE
    t = threading.Thread(target=loopfunction)
    t.start()
    print "WAITING FOR EDGE"
    GPIO.wait_for_edge("XIO-P0",GPIO.FALLING)
    print "WE'VE FALLEN LIKE COOLIO'S CAREER"
    print "ATTEMPTING TO CANCEL THE TIMER"
    t.cancel()
except:
    pass

print "TESTING ERRORS THROWN WHEN SPECIFYING EDGE DETECTION ON UNAUTHORIZED GPIO"
GPIO.setup("CSID1",GPIO.IN)
GPIO.add_event_detect("CSID1",GPIO.FALLING,myfuncallback)

print "CLEANUP"
GPIO.remove_event_detect("XIO-P0")
GPIO.cleanup()

