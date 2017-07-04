## CHIP_IO.OverlayManager
Import the OverlayManager module as follows

  ```python
  import CHIP_IO.OverlayManager as OM
  ```

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  OM.toggle_debug()
  ```

### get_spi_loaded()
Check to see if the SPI DTBO is loaded

* Parameters
  
  None

* Returns

  boolean

* Examples

  ```python
  is_spi_loaded = OM.get_spi_loaded()
  ```

### get_pwm_loaded()
Check to see if the PWM0 DTBO is loaded

* Parameters
  
  None

* Returns

  boolean

* Examples

  ```python
  is_pwm_loaded = OM.get_pwm_loaded()
  ```

### get_custom_loaded()
Check to see if the Custom DTBO is loaded

* Parameters
  
  None

* Returns

  boolean

* Examples

  ```python
  is_custom_loaded = OM.get_custom_loaded()
  ```

[home](./index.md)
