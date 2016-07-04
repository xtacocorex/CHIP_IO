#!/usr/bin/python

import CHIP_IO.GPIO as GPIO
import time
import threading

num_callbacks = 0


def myfuncallback(channel):
    global num_callbacks
    num_callbacks += 1
    print "CALLBACK LIKE DRAKE IN HOTLINE BLING"


loopfunction_exit = False


def loopfunction():
    print "LOOP FUNCTION"
    for i in xrange(4):
        if loopfunction_exit:
            break
        if i % 2:
            print "SETTING CSID0 LOW (i=", i, ")"
            GPIO.output("CSID0", GPIO.LOW)
        else:
            print "SETTING CSID0 HIGH (i=", i, ")"
            GPIO.output("CSID0", GPIO.HIGH)
        print "SLEEPING"
        time.sleep(1)

num_errs = 0

GPIO.selftest(0)

GPIO.setup("XIO-P0", GPIO.IN)
GPIO.setup("CSID0", GPIO.OUT, initial=GPIO.HIGH)
if (GPIO.input("XIO-P0") != GPIO.HIGH):
    print "A high output on CSI0 does not lead to a high input on XIO-P0."
    print "Perhaps you forgot to connect them?"
    num_errs += 1
GPIO.cleanup()

GPIO.setup("U14_13", GPIO.IN)  # XIO-P0
GPIO.setup("CSID0", GPIO.OUT, initial=GPIO.LOW)
if (GPIO.input("XIO-P0") != GPIO.LOW):
    print "A low output on CSI0 does not lead to a low input on XIO-P0."
    print "Perhaps you forgot to connect them?"
    num_errs += 1
GPIO.cleanup()

GPIO.setup("XIO-P0", GPIO.IN)
GPIO.setup("U14_31", GPIO.OUT)  # CSID0

# VERIFY SIMPLE FUNCTIONALITY
print "VERIFY SIMPLE FUNCTIONALITY"

print "READING XIO-PI"
GPIO.output("CSID0", GPIO.HIGH)
assert(GPIO.input("XIO-P0") == GPIO.HIGH)

GPIO.output("CSID0", GPIO.LOW)
print "LOW", GPIO.input("XIO-P0")
assert(GPIO.input("XIO-P0") == GPIO.LOW)

# ==============================================
# EDGE DETECTION - AP-EINT1
print "SETTING UP RISING EDGE DETECTION ON AP-EINT1"
GPIO.setup("AP-EINT1", GPIO.IN)
print "adding event detect"
GPIO.add_event_detect("AP-EINT1", GPIO.RISING)

print "VERIFYING EDGE DETECT"
f = open("/sys/class/gpio/gpio193/edge", "r")
edge = f.read()
f.close()
assert(edge == "rising\n")
GPIO.remove_event_detect("AP-EINT1")

# ==============================================
# EDGE DETECTION - AP-EINT3
print "SETTING UP BOTH EDGE DETECTION ON AP-EINT3"
GPIO.setup("AP-EINT3", GPIO.IN)
GPIO.add_event_detect("AP-EINT3", GPIO.BOTH)

print "VERIFYING EDGE DETECT"
f = open("/sys/class/gpio/gpio35/edge", "r")
edge = f.read()
f.close()
assert(edge == "both\n")
GPIO.remove_event_detect("AP-EINT3")

# ==============================================
# EDGE DETECTION - EXPANDED GPIO
print "SETTING UP FALLING EDGE DETECTION ON XIO-P0"
GPIO.add_event_detect("XIO-P0", GPIO.FALLING, myfuncallback)

print "VERIFYING EDGE DETECT"
base = GPIO.get_gpio_base()
gfile = "/sys/class/gpio/gpio%d/edge" % base
f = open(gfile, "r")
edge = f.read()
f.close()
assert(edge == "falling\n")

# LOOP WRITING ON CSID0 TO HOPEFULLY GET CALLBACK TO WORK
print "WAITING FOR CALLBACKS"
loopfunction()
print "num_callbacks=", num_callbacks

print "PRESS CONTROL-C TO EXIT IF SCRIPT GETS STUCK"
GPIO.remove_event_detect("XIO-P0")
try:
    # WAIT FOR EDGE
    t = threading.Thread(target=loopfunction)
    t.start()
    print "WAITING FOR EDGE"
    GPIO.wait_for_edge("XIO-P0", GPIO.FALLING)
    print "WE'VE FALLEN LIKE COOLIO'S CAREER"
except:
    pass

print "Exit thread"
loopfunction_exit = True
t.join()  # Wait till the thread exits.
GPIO.remove_event_detect("XIO-P0")

print "TESTING ERRORS THROWN WHEN SPECIFYING EDGE DETECTION ON UNAUTHORIZED GPIO"
GPIO.setup("CSID1", GPIO.IN)
try:
    GPIO.add_event_detect("CSID1", GPIO.FALLING, myfuncallback)
    print "Oops, it did not throw an exception!  BUG!!!"
    num_errors += 1
except ValueError, ex:
    print "error msg=", ex.args[0]
    pass
except RuntimeError, ex:
    print "error msg=", ex.args[0]
    pass

print "TESTING ERRORS THROWN WHEN SETTING UP AN ALREADY EXPORTED GPIO"
try:
    GPIO.output("XIO-P0", GPIO.LOW)
    print "Oops, it did not throw an exception!  BUG!!!"
    num_errors += 1
except ValueError, ex:
    print "error msg=", ex.args[0]
    pass
except RuntimeError, ex:
    print "error msg=", ex.args[0]
    pass

print "TESTING ERRORS THROWN WHEN WRITING TO A GPIO WITH NO DIRECTION"
try:
    GPIO.output("CSID1", GPIO.LOW)
    print "Oops, it did not throw an exception!  BUG!!!"
    num_errors += 1
except ValueError, ex:
    print "error msg=", ex.args[0]
    pass
except RuntimeError, ex:
    print "error msg=", ex.args[0]
    pass

print "TESTING ERRORS THROWN FOR ILLEGAL GPIO"
try:
    GPIO.setup("NOTUSED", GPIO.IN)
    print "Oops, it did not throw an exception!  BUG!!!"
    num_errors += 1
except ValueError, ex:
    print "error msg=", ex.args[0]
    pass
except RuntimeError, ex:
    print "error msg=", ex.args[0]
    pass

print "TESTING ERRORS THROWN FOR NON-GPIO"
try:
    GPIO.setup("FEL", GPIO.IN)
    print "Oops, it did not throw an exception!  BUG!!!"
    num_errors += 1
except ValueError, ex:
    print "error msg=", ex.args[0]
    pass
except RuntimeError, ex:
    print "error msg=", ex.args[0]
    pass

print "CLEANUP"
GPIO.cleanup()

print "done,", num_errs, " errors"
