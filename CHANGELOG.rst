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


