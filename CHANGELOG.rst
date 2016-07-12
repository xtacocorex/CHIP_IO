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


