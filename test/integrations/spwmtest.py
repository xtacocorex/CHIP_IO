#!/usr/bin/python

import CHIP_IO.SOFTPWM as SPWM
import CHIP_IO.GPIO as GPIO
import time
import datetime
import threading

class SPWMReceiver(threading.Thread):
    def __init__(self,gpio,key,maxcount=20,sleeptime=0.5):
        self.gpio = gpio
        self.key = key
        self.counter = 0
        self.maxcount = maxcount
        self.sleeptime = sleeptime
        threading.Thread.__init__(self)

    def run(self):
        print("SETTING UP RECEIVER GPIO")
        self.gpio.cleanup()
        self.gpio.setup(self.key, self.gpio.IN)
        print("STARTING RECEIVE LOOP")
        try:
            while self.counter < self.maxcount:
                pwmval = self.gpio.input(self.key)
                print("SPWM VALUE: {0} @ {1}".format(pwmval, datetime.datetime.now()))
                time.sleep(self.sleeptime)
                self.counter += 1
        except KeyboardInterrupt:
            self.gpio.cleanup(self.key)

if __name__ == "__main__":
    # SETUP VARIABLES
    SPWMGPIO = "XIO-P7"
    RECEIVERGPIO = "CSID0"
    COUNT = 200
    SLEEPTIME = 0.01

    # CLEANUP THE GPIO
    GPIO.cleanup()
    SPWM.cleanup()

    # ISSUE #16 VERIFICATION
    try:
        print("VERIFYING FIX FOR ISSUE #16, GPIO CONFIGURED THAT SPWM WANTS TO USE")
        GPIO.setup(SPWMGPIO, GPIO.OUT)
        SPWM.start(SPWMGPIO, 50, 1)
    except Exception as e:
        print("EXCEPTION: {}".format(e))
        print("GPIO CLEANUP")
        GPIO.cleanup()

    # SETUP SOFTPWM
    print("STARTING SOFTPWM TEST")
    SPWM.start(SPWMGPIO, 50, 1)
    SPWM.set_frequency(SPWMGPIO, 2)

    # SETUP SOFTPWM RECEIVER
    rcvr = SPWMReceiver(GPIO, RECEIVERGPIO, COUNT, SLEEPTIME)
    rcvr.start()

    time.sleep(COUNT*SLEEPTIME + 1)

    # CLEANUP
    print("CLEANUP")
    SPWM.stop(SPWMGPIO)
    SPWM.cleanup()
    GPIO.cleanup()


