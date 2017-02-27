#!/usr/bin/python

import CHIP_IO.SOFTPWM as PWM
import CHIP_IO.GPIO as GPIO
import CHIP_IO.OverlayManager as OM
import time
import datetime

if __name__ == "__main__":
    # SETUP VARIABLES
    PWMGPIO = "XIO-P7"
    #PWMGPIO = "LCD-D4"
    COUNT = 150
    SLEEPTIME = 0.01

    time.sleep(1)

    # SETUP PWM
    try:
        print("PWM START")
        #PWM.toggle_debug()
        PWM.start(PWMGPIO, 50, 45, 1)

        # UNCOMMENT FOR CRASH
        print("PWM SET FREQUENCY")
        PWM.set_frequency(PWMGPIO, 10)

        # UNCOMMENT FOR CRASH
        print("PWM SET DUTY CYCLE")
        PWM.set_duty_cycle(PWMGPIO, 25)

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


