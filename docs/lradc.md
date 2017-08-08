## CHIP_IO.LRADC
The LRADC module handles interfacing with the onboard 6-Bit, 2V tolerant ADC in the R8/GR8.

Import the LRADC module as follows

  ```python
  import CHIP_IO.LRADC as LRADC
  ```

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Returns

  None

* Examples

  ```python
  LRADC.toggle_debug()
  ```

### get_device_exist()
Check to see if the LRADC device exists

* Parameters
  
  None

* Returns

  boolean - True if LRADC is enabled, False is LRADC is disabled

* Examples

  ```python
  LRADC.get_device_exist()
  ```

### setup(rate=250)
Setup the LRADC, defaults to a sampling rate of 250.

* Parameters
  
  rate (optional) - Sampling rate of the LRADC: 32.25, 62.5, 125, 250

* Returns

  boolean - True if LRADC is enabled, False is LRADC is disabled

* Examples

  ```python
  LRADC.setup()
  LRADC.setup(32.25)
  ```

### get_scale_factor()
Get the scaling factor applied to raw values from the LRADC

* Parameters
  
  None

* Returns

  float - scale factor applied to the LRADC Raw data

* Examples

  ```python
  factor = LRADC.get_scale_factor()
  print(factor)
  ```

### get_allowable_sample_rates()
Get the allowable sample rates for the LRADC

* Parameters
  
  None

* Returns

  tuple - sampling rates of the LRADC

* Examples

  ```python
  rates = LRADC.get_allowable_sample_rates()
  print(rates)
  ```

### set_sample_rate(rate)
Set the current sample rates for the LRADC

* Parameters
  
  rate - Sample rate, only rates allowable by the LRADC

* Returns

  float - current sampling rate of the LRADC

* Examples

  ```python
  curr_rate = LRADC.set_sample_rate(125.0)
  ```

### get_sample_rate()
Get the current sample rates for the LRADC

* Parameters
  
  None

* Returns

  float - current sampling rate of the LRADC

* Examples

  ```python
  curr_rate = LRADC.get_sample_rate()
  ```

### get_chan0_raw()
Get the raw value for LRADC Channel 0

* Parameters
  
  None

* Returns

  float - current raw value of LRADC Channel 0

* Examples

  ```python
  dat = LRADC.get_chan0_raw()
  ```

### get_chan1_raw()
Get the raw value for LRADC Channel 1

* Parameters
  
  None

* Returns

  float - current raw value of LRADC Channel 1

* Examples

  ```python
  dat = LRADC.get_chan1_raw()
  ```

### get_chan0()
Get the value for LRADC Channel 0

* Parameters
  
  None

* Returns

  float - current value of LRADC Channel 0

* Examples

  ```python
  dat = LRADC.get_chan0()
  ```

### get_chan1
Get the value for LRADC Channel 1

* Parameters
  
  None

* Returns

  float - current value of LRADC Channel 1

* Examples

  ```python
  dat = LRADC.get_chan1()
  ```

[home](./index.md)
