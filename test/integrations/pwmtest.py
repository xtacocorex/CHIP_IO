#!/usr/bin/python

import CHIP_IO.PWM as PWM
import CHIP_IO.GPIO as GPIO
import CHIP_IO.OverlayManager as OM
import time
import datetime
import threading

class PWMReceiver(threading.Thread):
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
                print("PWM VALUE: {0} @ {1}".format(pwmval, datetime.datetime.now()))
                time.sleep(self.sleeptime)
                self.counter += 1
        except KeyboardInterrupt:
            self.gpio.cleanup(self.key)

def PrintPwmData():
    print("PRINTING PWM SYSFS DATA")
    f = open("/sys/class/pwm/pwmchip0/pwm0/enable")
    print("PWM0 ENABLE:\t{}".format(f.readline()))
    f.close()
    f = open("/sys/class/pwm/pwmchip0/pwm0/period")
    print("PWM0 PERIOD:\t{}".format(f.readline()))
    f.close()
    f = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle")
    print("PWM0 DUTY CYCLE:\t{}".format(f.readline()))
    f.close()
    f = open("/sys/class/pwm/pwmchip0/pwm0/polarity")
    print("PWM0 POLARITY:\t{}".format(f.readline()))
    f.close()

if __name__ == "__main__":
    # SETUP VARIABLES
    PWMGPIO = "PWM0"
    RECEIVERGPIO = "CSID0"
    COUNT = 150
    SLEEPTIME = 0.01

    # LOAD THE PWM OVERLAY
    print("LOADING PWM OVERLAY")
    OM.load("PWM0")

    time.sleep(1)

    # CLEANUP THE GPIO
    #GPIO.cleanup()
    #PWM.cleanup()

    # SETUP PWM
    try:
        print("PWM START")
        PWM.toggle_debug()
        PWM.start(PWMGPIO, 15, 50, 1)
        PrintPwmData()

        # UNCOMMENT FOR CRASH
        #print("PWM SET FREQUENCY")
        #PWM.set_frequency(PWMGPIO, 200)
        #PrintPwmData()

        # UNCOMMENT FOR CRASH
        #print("PWM SET DUTY CYCLE")
        #PWM.set_duty_cycle(PWMGPIO, 25)
        #PrintPwmData()

        # SETUP PWM RECEIVER
        #rcvr = PWMReceiver(GPIO, RECEIVERGPIO, COUNT, SLEEPTIME)
        #rcvr.start()

        #time.sleep(COUNT*SLEEPTIME + 1)
        raw_input("PRESS ENTER WHEN DONE")

    except:
        raise
    finally:
        # CLEANUP
        print("CLEANUP")
        PWM.stop(PWMGPIO)
        PWM.cleanup()
        #OM.unload("PWM0")
        #GPIO.cleanup()


