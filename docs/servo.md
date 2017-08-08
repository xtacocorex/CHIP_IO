## CHIP_IO.SERVO
Import the SERVO module as follows

  ```python
  import CHIP_IO.SERVO as SERVO
  ```

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Examples

  ```python
  SERVO.toggle_debug()
  ```

### is_chip_pro()
Function to report to the calling script if the SBC is a CHIP or a CHIP Pro

* Parameters
  
  None

* Returns

  int - 1 for CHIP Pro, 0 for CHIP

* Examples

  ```python
  is_chip_pro = UT.is_chip_pro()
  ```

### start(channel, angle=0.0, range=180.0)
Start the Servo

* Parameters
  
  channel - Pin servo is attached to
  angle - initial angle of the servo (optional)
  range - total range of the servo in degrees (optional)

* Returns

  None

* Examples

  ```python
  SERVO.start("CSID0")
  SERVO.start("CSID0", 37.0)
  SERVO.start("CSID0", -45.0, 180.0)
  ```

### stop(channel)
Stop the Servo

* Parameters
  
  channel - Pin servo is attached to

* Returns

  None

* Examples

  ```python
  SERVO.stop("CSID0")
  ```

### set_range(channel, range)
Set the range of the Servo

* Parameters
  
  channel - Pin servo is attached to
  range - total range of the servo in degrees

* Returns

  None

* Examples

  ```python
  SERVO.set_range("CSID0", 180.0)
  SERVO.set_range("CSID0", 360.0)
  ```

### set_angle(channel, angle)
Set the angle of the Servo

* Parameters
  
  channel - Pin servo is attached to
  angle - angle to set the servo between +/- 1/2*Range

* Returns

  None

* Examples

  ```python
  SERVO.set_angle("CSID0", -45.0)
  SERVO.set_angle("CSID0", 36.0)
  ```

### cleanup()
Cleanup all setup Servos, this will blast away every sero currently in operation

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  SERVO.cleanup()
  ```

[home](./index.md)
