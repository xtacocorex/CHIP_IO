## CHIP_IO.SOFTPWM
Import the SOFTPWM module as follows

  ```python
  import CHIP_IO.SOFTPWM as SPWM
  ```

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Examples

  ```python
  SPWM.toggle_debug()
  ```

### is_chip_pro()
Function to report to the calling script if the SBC is a CHIP or a CHIP Pro

* Parameters
  
  None

* Returns

  int - 1 for CHIP Pro, 0 for CHIP

* Examples

  ```python
  is_chip_pro = SPWM.is_chip_pro()
  ```

### start(channel, duty_cycle=0.0, frequency=2000.0, polarity=0)
Start the Software PWM

* Parameters
  
  channel - pin for software PWM is configured 
  duty_cycle - initial duty cycle of the PWM (optional)
  frequency - frequency of the PWM (optional)
  polarity - signal polarity of the PWM (optional)

* Returns

  None

* Examples

  ```python
  SPWM.start("CSID0")
  SPWM.start("CSID0", 37.0)
  SPWM.start("CSID0", 10.0, 500.0)
  SPWM.start("CSID0", 50.0, 1000.0, 1)
  ```

### stop(channel)
Stop the Software PWM

* Parameters
  
  channel - pin software PWM is configured 

* Returns

  None

* Examples

  ```python
  SPWM.stop("CSID0")
  ```

### set_duty_cycle(channel, duty_cycle)
Set the duty cycle of the Software PWM

* Parameters
  
  channel - pin software PWM is configured 
  duty_cycle - duty cycle of the PWM (0.0 to 100.0)

* Returns

  None

* Examples

  ```python
  SPWM.set_duty_cycle("CSID0", 25.0)
  ```

### set_frequency(channel, frequency)
Set the frequency of the Software PWM in Hertz

* Parameters
  
  channel - pin PWM is configured
  frequency - frequency of the PWM

* Returns

  None

* Examples

  ```python
  SPWM.set_frequency("CSID0", 450.0)
  ```

### cleanup(channel)
Cleanup Software PWM.  If not channel input, all Software PWM will be cleaned up

* Parameters
  
  channel - pin Software PWM is configured (optional)

* Returns

  None

* Examples

  ```python
  SPWM.cleanup()
  SPWM.cleanup("CSID0")
  ```

[home](./index.md)
