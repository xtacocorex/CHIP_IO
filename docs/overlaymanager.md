## CHIP_IO.OverlayManager
Import the OverlayManager module as follows

  ```python
  import CHIP_IO.OverlayManager as OM
  ```

This module requires NTC's [CHIP-dt-overlays](https://github.com/NextThingCo/CHIP-dt-overlays) to be loaded.

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

### load(overlay, path="")
Loads the overlay specified.  PWM0 is not available on the CHIP Pro due to the base DTS supporting both PWM0 and PWM1

* Parameters
  
  overlay - Overlay to be loaded: SPI2, PWM0, CUST
  path (optional) - Path to the custom compiled overlay

* Returns

  integer - 0: Success, 1: Fail, 2: Overlay already loaded

* Examples

  ```python
  resp = OM.load("SPI2")
  resp = OM.load("PWM0")
  resp = OM.load("CUST","path/to/custom.dtbo")
  ```

### unload(overlay)
Unloads the overlay specified.  PWM0 is not available on the CHIP Pro due to the base DTS supporting both PWM0 and PWM1

* Parameters
  
  overlay - Overlay to be loaded: SPI2, PWM0, CUST

* Returns

  None

* Examples

  ```python
  resp = OM.unload("SPI2")
  resp = OM.unload("PWM0")
  resp = OM.unload("CUST")
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
