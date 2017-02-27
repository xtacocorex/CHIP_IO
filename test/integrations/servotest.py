import CHIP_IO.SERVO as SERVO
import CHIP_IO.GPIO as GPIO
import time
import datetime
import threading

class ServoTestReceiver(threading.Thread):
    def __init__(self,gpio,key,maxcount=20,sleeptime=0.005):
        self.gpio = gpio
        self.key = key
        self.counter = 0
        self.maxcount = maxcount
        self.sleeptime = sleeptime
        self.dead = False
        threading.Thread.__init__(self)

    def kill(self):
        self.dead = True

    def run(self):
        print("SETTING UP RECEIVER GPIO")
        self.gpio.cleanup()
        self.gpio.setup(self.key, self.gpio.IN)
        print("STARTING RECEIVE LOOP")
        try:
            #while self.counter < self.maxcount:
            while not self.dead:
                pwmval = self.gpio.input(self.key)
                print("SERVO VALUE: {0} @ {1}".format(pwmval, datetime.datetime.now()))
                time.sleep(self.sleeptime)
                self.counter += 1
        except KeyboardInterrupt:
            self.gpio.cleanup(self.key)


if __name__ == "__main__":
    # SETUP VARIABLES
    SERVOGPIO = "CSID1" #"XIO-P7"
    RECEIVERGPIO = "CSID7"
    COUNT = 120
    SLEEPTIME = 0.0001
    RANGE = 180

    # SETUP PWM
    try:
        print("SERVO START")
        SERVO.toggle_debug()
        SERVO.start(SERVOGPIO, 0, RANGE)

        # SETUP SERVO RECEIVER
        #rcvr = ServoTestReceiver(GPIO, RECEIVERGPIO, COUNT, SLEEPTIME)
        #rcvr.start()

        # LOOP THROUGH RANGE
        for i in range(-(RANGE/2),(RANGE/2),5):
            print("SETTING ANGLE: {0}".format(i))
            SERVO.set_angle(SERVOGPIO, i)
            time.sleep(1)
        
        #rcvr.kill()
        raw_input("PRESS ENTER WHEN DONE")

    except:
        raise
    finally:
        # CLEANUP
        print("CLEANUP")
        SERVO.stop(SERVOGPIO)
        SERVO.cleanup(SERVOGPIO)


