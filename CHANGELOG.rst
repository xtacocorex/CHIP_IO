0.7.1
---
* Merged in PR #79
* Merged in PR #80
* Added message notifying user of the gpio set direction retry

0.7.0
---
* Added ability to specify GPIO only as a number, this doesn't work for PWM/SPWM/LRADC/SERVO

0.6.2
---
* Implementation for #77 - ability to push up binary pypi
* Implementation for #75 - wait_for_edge timeout

0.6.1
---
* Fixing implementation for #76

0.6
---
* Random comment cleanup
* Implement fix for #76
* API documentation added
* Closing #74

0.5.9
---
* Merged PR#70 to enable the underlying C code to be used properly in C based code
* Updated README to add missing pins on the CHIP Pro that are available as GPIO
* Updated README to denote pins that are available for Edge Detection

0.5.8
---
* Added 3 pins for the CHIP Pro as allowable for setting callbacks and edge detection to close out Issue #68

0.5.7
---
* Added the I2S pins on the CHIP Pro as GPIO capable
* Added per PWM/SoftPWM cleanup per Issue #64

0.5.6
---
* Fix for Issue #63 where re-setting up a pin wasn't lining up with RPi.GPIO standards. Calling setup after the first time will now update direction.
* README updates to point out the direction() function since that was missing

0.5.5
---
* Fix for Issue #62 where using alternate name of an XIO would cause a segfault due to trying to set pull up/down resistor setting

0.5.4
---
* Re-enabled the polarity setting for PWM based upon Issue #61
* Fixed a 1 letter bug was trying to write inverted to polarity when it wants inversed (such facepalm)
* Cleaned up the polarity setting code to work when PWM is not enabled
* Fixed the unit test for pwm to verify we can set polarity

0.5.3
---
* Fixes to the PWM pytest
* Added pytest for LRADC and Utilities
* Makefile updates for all the things

0.5.2
---
* Updating Utilties to determine CHIP Pro better
* Updating the README to fix things

0.5.0
---
* CHIP Pro Support
* README Updates

0.4.0
---
* Software Servo code added
  - Only works on the LCD and CSI pins
* Fixed cleanup() for the SOFTPWM and SERVO
  - The per pin cleanup for SOFTPWM doesn't work as stop() clears up the memory for the pin used
  - SERVO code was based on SOFTPWM, so it inherited this issue

0.3.5
---
* Merged in brettcvz's code to read a byte of data from the GPIO
  - Cleaned the code up and expanded it (in the low level C code) to read up to 32 bits of data
  - Presented 8 bit and 16 bits of data functions to the Python interface with brettcvz's read_byte() and my read_word()
* I think I finally fixed the GPIO.cleanup() code one and for all

0.3.4.1
---
* Quick fix as I borked XIO setup as inputs with the latest change that enabled PUD

0.3.4
---
* Pull Up/Pull Down resistor setting now available for the R8 GPIO.
* Some general cleanup

0.3.3
----
* Added Debug printing for all the capabilities with the toggle_debug() function
* Added 2 functions from @streamnsight for PWM that allow for setting the period of the PWM and the Pulse Width, both in nanoseconds
* Fixed the SPI2 overlay stuff by using the NTC overlay instead of mine.

0.3.2
----
* Fixing issue #53 to handle the return values of the set functions in pwm_enable.
* Start of whole library debug for #55

0.3.1
----
* Fixing issue #50 where I broke GPIO.cleanup() and SOFTPWM.cleanup() when no input is specified.

0.3.0
----
* Added setmode() function for GPIO to maintain compatibility with Raspberry Pi scripts, this function literally does nothing
* Added per pin cleanup functionality for GPIO and SoftPWM so you can unexport a pin without unexporting every pin
* Updated README to make edge detection wording a little better and to add the per pin cleanup code
* Version update since I blasted through 3 issues on github and feel like we need a nice bump to 0.3

0.2.7
----
* Fix to the Enable 1.8V Pin code as it wasn't working due to bit shifting isn't allowed on a float.
* Updated README to denote the PocketCHIP Pin names better

0.2.6
----
* Fix to keep the GPIO value file open until the pin is unexported (issue #34)

0.2.5
----
* Updates to the pytest code for HWPWM and SoftPWM
* Removed the i2c-1 load/unload support in OverlayManager as CHIP Kernel 4.4.13 has that bus brought back by default

0.2.4
----
* HW PWM Fixed
  - Start/Stop/Duty Cycle/Frequency settings work
  - Polarity cannot be changed, so don't bother setting it to 1 in start()
* Added the unexport_all() function to Utilites

0.2.3
----
* LRADC Support
* Added Utilities
  - Enable/Disable the 1.8V Pin
  - Change 1.8V Pin to output either 2.0V, 2.6V, or 3.3V
    (Current limited to 50mA)

0.2.2
----
* Fixes for Issue #16
  - Pass SoftPWM setup errors to Python layer (aninternetof)
  - Updated spwmtest.py to test for this issue

0.2.1
----
* Pull request #12 fixes:
 - Fixed indent in the i2c-1 dts
 - Removed import dependencies in the SPI and PWM overlays
 - Re-enabled building of the dtbo on setup.py install

0.2.0
----
* Added the ability to load DTB Overlays from within CHIP_IO
 - Support for PWM0, SPI2, and I2C-1 (which comes back as i2c-3 on the 4.4 CHIP
 - Support for a custom DTB Overlay
* Fixes to the pwm unit test, all but 2 now pass :)

0.1.2
----
* SoftPWM Fix by aninternetof
* Added a verification test for SoftPWM

0.1.1
----
* Some refactoring of the edge detection code, made it function better
* Added Rising and Both edge detection tests to gptest.py
  - Small issue with both edge triggering double pumping on first callback hit

0.1.0
----
* Fixed edge detection code, will trigger proper for callbacks now

0.0.9
----
* Fixed SoftPWM segfault
* Added Alternate Names for the GPIOs

0.0.8
----
* Updates to handle the 4.4 kernel CHIPs.  Numerous fixes to fix code issues.
* Added ability to get the XIO base into Python.
* Still need a proper overlay for Hardware PWM and SPI.

0.0.7
----
* GPIO edge detection expanded to include AP-EINT1 and AP-EINT3 as those are the only other pins that support edge detection

0.0.6
----
* Initial PWM
* GPIO edge detection and callback for XIO-P0 to XIO-P7 working

0.0.4
____
* Initial Commit
* GPIO working - untested callback and edge detection
* Initial GPIO unit tests


