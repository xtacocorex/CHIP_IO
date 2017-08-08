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

[home](./index.md)
