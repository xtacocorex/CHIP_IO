## CHIP_IO.Utilities
Import the Utilities module as follows

  ```python
  import CHIP_IO.Utilities as UT
  ```

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  UT.toggle_debug()
  ```

### unexport_all()
Function to force clean up all exported GPIO on the system

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  UT.unexport_all()
  ```

### is_chip_pro()
Function to report to the calling script if the SBC is a CHIP or a CHIP Pro

* Parameters
  
  None

* Returns

  boolean - True for CHIP Pro, False for CHIP

* Examples

  ```python
  is_chip_pro = UT.is_chip_pro()
  ```

### enable_1v8_pin()
Enable the 1.8V pin on the CHIP as it is disabled by default.  Also sets the output to 1.8V.
This only works on the CHIP.

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  UT.enable_1v8_pin()
  ```

### set_1v8_pin_voltage(voltage)
Change the voltage of the 1.8V Pin on the CHIP.
This only works on the CHIP.

* Parameters
  
  voltage - 1.8, 2.0, 2.6, 3.3

* Returns

  boolean - False on error

* Examples

  ```python
  UT.set_1v8_pin_voltage(2.0)
  ```


### get_1v8_pin_voltage(voltage)
Get the current voltage of the 1.8V Pin on the CHIP.
This only works on the CHIP.

* Parameters
  
  None

* Returns

  float - current voltage of the 1.8V Pin

* Examples

  ```python
  volts = UT.get_1v8_pin_voltage()
  ```

### disable_1v8_pin()
Disables the 1.8V pin on the CHIP.
This only works on the CHIP.

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  UT.disable_1v8_pin()
  ```

[home](./index.md)
