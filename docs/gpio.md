## CHIP_IO.GPIO
Import the GPIO module as follows

  ```python
  import CHIP_IO.GPIO as GPIO
  ```

Note: As of version 0.7.0, all GPIO functions can use the SYSFS pin number for a GPIO for control, a la RPi.GPIO.

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Examples

  ```python
  GPIO.toggle_debug()
  ```

### is_chip_pro()
Function to report to the calling script if the SBC is a CHIP or a CHIP Pro

* Parameters
  
  None

* Returns

  int - 1 for CHIP Pro, 0 for CHIP

* Examples

  ```python
  is_chip_pro = GPIO.is_chip_pro()
  ```

### setmode(mode)
Dummy function to maintain backwards compatibility with Raspberry Pi scripts.

### setup(channel, direction, pull_up_down=PUD_OFF, initial=None)
Setup a GPIO pin.  If pin is already configure, it will reconfigure.

* Parameters
  
  channel - GPIO pin
  direction - INPUT or OUTPUT
  pull_up_down - PUD_OFF, PUD_UP, PUD_DOWN (optional)
  initial - Initial value for an OUTPUT pin (optional)

* Returns

  None

* Examples

  ```python
  GPIO.setup("CSID0", GPIO.IN)
  GPIO.setup(132, GPIO.IN)
  GPIO.setup("CSID3", GPIO.OUT, initial=1)
  GPIO.setup("CSID2", GPIO.IN, GPIO.PUD_UP)
  ```

### cleanup(channel)
Cleanup GPIO.  If not channel input, all GPIO will be cleaned up

* Parameters
  
  channel - GPIO pin (optional)

* Returns

  None

* Examples

  ```python
  GPIO.cleanup()
  GPIO.cleanup("CSID3")
  GPIO.cleanup(132)
  ```

### output(channel, value)
Write a value to a GPIO pin.

* Parameters
  
  channel - GPIO Pin
  value - HIGH, LOW, 0, 1

* Returns

  None

* Examples

  ```python
  GPIO.output("XIO-P7", GPIO.HIGH)
  GPIO.output("XIO-P7", GPIO.LOW)
  GPIO.output("CSID0", 1)
  GPIO.output("CSID0", 0)
  GPIO.output(132, 1)
  ```

### input(channel)
Read a GPIO pin once.

* Parameters
  
  channel - GPIO Pin

* Returns

  value - current value of the GPIO pin

* Examples

  ```python
  value = GPIO.input("XIO-P7")
  value = GPIO.input(1013)
  ```

### read_byte(channel)
Read a GPIO pin multiple times to fill up 8 bits.

* Parameters
  
  channel - GPIO Pin

* Returns

  int - 8 bit value of the GPIO pin

* Examples

  ```python
  bits = GPIO.read_byte("XIO-P7")
  bits = GPIO.read_byte(135)
  ```

### read_word(channel)
Read a GPIO pin multiple times to fill up 16 bits.

* Parameters
  
  channel - GPIO Pin

* Returns

  word - 16 bit value of the GPIO pin

* Examples

  ```python
  bits = GPIO.read_word("XIO-P7")
  bits = GPIO.read_word(134)
  ```

### add_event_detect(channel, edge, callback=None, bouncetime=0)
Add event detection to a pin. Refer to main table for which pins are able to use edge detection.

* Parameters
  
  channel - GPIO Pin
  edge - edge: RISING_EDGE, FALLING_EDGE, BOTH_EDGE
  callback - callback function to be run when edge is detected (optional)
  bouncetime - level debounce time period in ms (optional)

* Returns

  None

* Examples

  ```python
  GPIO.add_event_detect("XIO-P7", GPIO.RISING_EDGE)
  GPIO.add_event_detect("AP-EINT3", GPIO.RISING_EDGE, mycallback)
  GPIO.add_event_detect("XIO-P7", GPIO.FALLING_EDGE, bouncetime=30)
  GPIO.add_event_detect("XIO-P7", GPIO.RISING_EDGE, mycallback, 45)
  GPIO.add_event_detect(1013, GPIO.BOTH_EDGE)
  ```

### remove_event_detect(channel)
Remove a pins event detection. Refer to main table for which pins are able to use edge detection.

* Parameters
  
  channel - GPIO Pin

* Returns

  None

* Examples

  ```python
  GPIO.remove_event_detect("XIO-P7")
  GPIO.remove_event_detect(1013)
  ```

### event_detected(channel)
Function to determine if an event was detected on a pin.  Pin must have an event detect added via add_event_detect() prior to calling this function. Refer to main table for which pins are able to use edge detection.

* Parameters
  
  channel - GPIO Pin

* Returns

  boolean - True if event was detected

* Examples

  ```python
  have_event = GPIO.event_detected("XIO-P5")
  have_event = GPIO.event_detected(1014)
  ```

### add_event_callback(channel, callback, bouncetime=0)
Add callback function to a pin that has been setup for edge detection. Refer to main table for which pins are able to use edge detection.

* Parameters
  
  channel - GPIO Pin
  callback - callback function to be run when edge is detected
  bouncetime - level debounce time period in ms (optional)

* Returns

  None

* Examples

  ```python
  GPIO.add_event_callback("AP-EINT3", mycallback)
  GPIO.add_event_callback("XIO-P7", mycallback, 45)
  GPIO.add_event_callback(1013, mycallback)
  ```

### wait_for_edge(channel, edge, timeout=-1)
Wait for an edge to be detected.  This is a blocking function. Refer to main table for which pins are able to use edge detection.

* Parameters
  
  channel - GPIO Pin
  edge - edge: RISING_EDGE, FALLING_EDGE, BOTH_EDGE
  timeout - timeout in milliseconds to wait before exiting function (optional)

* Returns

  None

* Examples

  ```python
  GPIO.wait_for_edge("XIO-P3", GPIO.RISING_EDGE)
  GPIO.wait_for_edge("AP-EINT3", GPIO.BOTH_EDGE)
  GPIO.wait_for_edge("I2S-DI", GPIO.FALLING_EDGE)
  GPIO.wait_for_edge("XIO-P3", GPIO.RISING_EDGE, 40)
  GPIO.wait_for_edge(1013, GPIO.BOTH_EDGE, 35)
  ```

### gpio_function(channel)
Function to report get a GPIO Pins directioj

* Parameters
  
  channel - GPIO Pin

* Returns

  int - GPIO Pin direction

* Examples

  ```python
  funct = GPIO.gpio_function("CSID0")
  funct = GPIO.gpio_function(132)
  ```

### setwarnings(state)
Function to enable/disable warning print outs. This may or may not work properly.  toggle_debug() is a better bet.

* Parameters
  
  state - 1 for enable, 0 for disable

* Returns

  None

* Examples

  ```python
  GPIO.set_warnings(1)
  ```

### get_gpio_base()
Function to get the SYSFS base value for the XIO pins on a CHIP

* Parameters
  
  None

* Returns

  int - sysfs base of the XIO, returns -1 on a CHIP Pro

* Examples

  ```python
  base = GPIO.get_gpio_base()
  ```

### selftest(value)
Function to perform a selftest on the GPIO module

* Parameters
  
  value - a value

* Returns

  int - the input value

* Examples

  ```python
  rtn = GPIO.selftest(0)
  ```

### direction(channel, direction)
Function to set the direction of an exported GPIO pin

* Parameters
  
  channel - GPIO Pin
  direction - Direction Pin is to take

* Returns

  None

* Examples

  ```python
  GPIO.set_direction("XIO-P0", GPIO.OUT)
  GPIO.set_direction("XIO-P1", GPIO.IN)
  GPIO.set_direction(1013, GPIO.OUT)
  ```

[home](./index.md)
